#include <FastLED.h>
#include <FlashStorage.h>

/**
 * LED index to button mapping
 *               O
 * O O O O     O
 *           O
 * O O O O
 *         O
 */

// #define DEBUG

// For FeatherWing OLED
#define DATA_PIN 6

// For Seeeduino XIAO
// #define DATA_PIN 10

#define BUTTON_PIN 9
#define BUTTON_COUNT 12
#define LEDS_PER_BUTTON 2
#define LED_COUNT (BUTTON_COUNT * LEDS_PER_BUTTON)
#define EFFECT_COUNT 12
#define COLUMN_COUNT 7
#define COLUMN_HEIGHT 3
#define BRIGHTNESS 20
#define CHASE_DELAY 10

#define BUTTON_INDEX_UP      0
#define BUTTON_INDEX_K1      1
#define BUTTON_INDEX_K2      2
#define BUTTON_INDEX_K3      3
#define BUTTON_INDEX_K4      4
#define BUTTON_INDEX_P4      5
#define BUTTON_INDEX_P3      6
#define BUTTON_INDEX_P2      7
#define BUTTON_INDEX_P1      8
#define BUTTON_INDEX_RIGHT   9
#define BUTTON_INDEX_DOWN   10
#define BUTTON_INDEX_LEFT   11
#define BUTTON_NONE        255

// Reserve a portion of flash memory to store the selected effect
FlashStorage(selectedEffectStore, uint8_t);

// State variables
CRGB leds[LED_COUNT];
volatile uint8_t selectedEffect = 0;
volatile bool staticOn = false;
volatile bool effectChanged = false;

// Define mappings for HitBox button layout
static const uint8_t ColumnMapping[COLUMN_COUNT][COLUMN_HEIGHT] = {
	{ BUTTON_INDEX_LEFT,  BUTTON_NONE,     BUTTON_NONE },
	{ BUTTON_INDEX_DOWN,  BUTTON_NONE,     BUTTON_NONE },
	{ BUTTON_INDEX_RIGHT, BUTTON_NONE,     BUTTON_NONE },
	{ BUTTON_INDEX_P1,    BUTTON_INDEX_K1, BUTTON_INDEX_UP },
	{ BUTTON_INDEX_P2,    BUTTON_INDEX_K2, BUTTON_NONE },
	{ BUTTON_INDEX_P3,    BUTTON_INDEX_K3, BUTTON_NONE },
	{ BUTTON_INDEX_P4,    BUTTON_INDEX_K4, BUTTON_NONE },
};

// Used by rainbowCycle and theaterChaseRainbow
byte * Wheel(byte position) {
	static byte c[3];
	
	if (position < 85) {
		c[0] = position * 3;
		c[1] = 255 - position * 3;
		c[2] = 0;
	} else if (position < 170) {
		position -= 85;
		c[0] = 255 - position * 3;
		c[1] = 0;
		c[2] = position * 3;
	} else {
		position -= 170;
		c[0] = 0;
		c[1] = position * 3;
		c[2] = 255 - position * 3;
	}

	return c;
}

void setup() {
#ifdef DEBUG
	Serial.begin(115200);
	while (!Serial);
	Serial.println("Attached");
#endif
	// Configure LEDs
	FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, LED_COUNT);

	// Set brightness
	FastLED.setBrightness(BRIGHTNESS);

	// Get saved effect state
	selectedEffect = selectedEffectStore.read();

	// Initial display
	show();

	// Set up change effect button
	pinMode(BUTTON_PIN, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), changeEffect, CHANGE);
}

void loop() {
	effectChanged = false;
	switch (selectedEffect) {
		case  0:  effectOff();                             break;
		case  1:  effectRainbowStatic();                   break;
		case  2:  effectRainbowChase(CHASE_DELAY);         break;
		case  3:  effectRainbowChaseColumns(CHASE_DELAY);  break;
		case  4:  effectStaticColor(CRGB::White);          break;
		case  5:  effectStaticColor(CRGB::Red);            break;
		case  6:  effectStaticColor(CRGB::Orange);         break;
		case  7:  effectStaticColor(CRGB::Yellow);         break;
		case  8:  effectStaticColor(CRGB::Green);          break;
		case  9:  effectStaticColor(CRGB::Aqua);           break;
		case 10:  effectStaticColor(CRGB::Blue);           break;
		case 11:  effectStaticColor(CRGB::Purple);         break;
	}
}

void changeEffect() {
	if (digitalRead(BUTTON_PIN) == LOW) {
		selectedEffect++;
		if (selectedEffect >= EFFECT_COUNT)
			selectedEffect = 0;
#ifdef DEBUG
		Serial.print("Changing effect to ");
		Serial.println(selectedEffect);
#endif
		selectedEffectStore.write(selectedEffect);
		staticOn = false;
		effectChanged = true;
	}
}

// *******************
// ** LED Functions **
// *******************

void show() {
	FastLED.show();
}

void setLED(int ledIndex, byte red, byte green, byte blue) {
	leds[ledIndex].r = red;
	leds[ledIndex].g = green;
	leds[ledIndex].b = blue;
}

void setLED(int ledIndex, CRGB rgbColor) {
	leds[ledIndex] = rgbColor;
}

void setColumn(int colIndex, byte red, byte green, byte blue) {
	for (uint8_t i = 0; i < COLUMN_HEIGHT; i++) {
		if (ColumnMapping[colIndex][i] != BUTTON_NONE) {
			for (uint8_t j = 0; j < LEDS_PER_BUTTON; j++) {
				leds[(ColumnMapping[colIndex][i] * LEDS_PER_BUTTON) + j].r = red;
				leds[(ColumnMapping[colIndex][i] * LEDS_PER_BUTTON) + j].g = green;
				leds[(ColumnMapping[colIndex][i] * LEDS_PER_BUTTON) + j].b = blue;
			}
		}
	}
}

void setColumn(int colIndex, CRGB rgbColor) {
	for (uint8_t i = 0; i < COLUMN_HEIGHT; i++) {
		if (ColumnMapping[colIndex][i] != BUTTON_NONE)
			for (uint8_t j = 0; j < LEDS_PER_BUTTON; j++)
				leds[(ColumnMapping[colIndex][i] * LEDS_PER_BUTTON) + j] = rgbColor;
	}
}

void setAll(byte red, byte green, byte blue) {
	for (int i = 0; i < LED_COUNT; i++ ) {
		setLED(i, red, green, blue);
	}
}

void setAll(CRGB rgbColor) {
	for (int i = 0; i < LED_COUNT; i++ ) {
		setLED(i, rgbColor);
	}
}

// **************************
// ** LED Effect Functions **
// **************************

/**
 * Basic effects
 */
void effectOff() {
	if (!staticOn) {
		setAll(0, 0, 0);
		show();
		staticOn = true;
	}
}

void effectStaticColor(CRGB rgbColor) {
	if (!staticOn) {
		setAll(rgbColor);
		show();
		staticOn = true;
	}
}

/**
 * Rainbow effects
 */
void effectRainbowChase(int delayTime) {
	byte *c;
	uint16_t i, j;
	for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
		for (i = 0; i < LED_COUNT; i++) {
			if (effectChanged)
				return;

			c = Wheel(((i * 256 / LED_COUNT) + j) & 255);
			setLED(i, *c, *(c + 1), *(c + 2));
		}
		show();
		delay(delayTime);
	}
}

void effectRainbowChaseColumns(int delayTime) {
	byte *c;
	uint16_t i, j;
	for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
		for (i = 0; i < COLUMN_COUNT; i++) {
			if (effectChanged)
				return;

			c = Wheel(((i * 256 / COLUMN_COUNT) + j) & 255);
			setColumn(i, *c, *(c + 1), *(c + 2));
		}
		show();
		delay(delayTime);
	}
}

void effectRainbowStatic() {
	if (!staticOn) {
		for (uint8_t i = 0; i < LEDS_PER_BUTTON; i++) {
			leds[(BUTTON_INDEX_UP    * LEDS_PER_BUTTON) + i] = CRGB::Green;
			leds[(BUTTON_INDEX_K1    * LEDS_PER_BUTTON) + i] = CRGB::Green;
			leds[(BUTTON_INDEX_K2    * LEDS_PER_BUTTON) + i] = CRGB::Aqua;
			leds[(BUTTON_INDEX_K3    * LEDS_PER_BUTTON) + i] = CRGB::Blue;
			leds[(BUTTON_INDEX_K4    * LEDS_PER_BUTTON) + i] = CRGB::Purple;
			leds[(BUTTON_INDEX_P4    * LEDS_PER_BUTTON) + i] = CRGB::Purple;
			leds[(BUTTON_INDEX_P3    * LEDS_PER_BUTTON) + i] = CRGB::Blue;
			leds[(BUTTON_INDEX_P2    * LEDS_PER_BUTTON) + i] = CRGB::Aqua;
			leds[(BUTTON_INDEX_P1    * LEDS_PER_BUTTON) + i] = CRGB::Green;
			leds[(BUTTON_INDEX_RIGHT * LEDS_PER_BUTTON) + i] = CRGB::Yellow;
			leds[(BUTTON_INDEX_DOWN  * LEDS_PER_BUTTON) + i] = CRGB::Orange;
			leds[(BUTTON_INDEX_LEFT  * LEDS_PER_BUTTON) + i] = CRGB::Red;
		}
		show();
		staticOn = true;
	}
}
