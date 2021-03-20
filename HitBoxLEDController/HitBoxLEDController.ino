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
#define LED_COUNT (BUTTON_COUNT * 2)
#define EFFECT_COUNT 5
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
		case 0:  effectOff();                             break;
		case 1:  effectRainbowStatic();                   break;
		case 2:  effectRainbowChase(CHASE_DELAY);         break;
		case 3:  effectRainbowChaseColumns(CHASE_DELAY);  break;
		case 4:  effectWhite();                           break;
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
	for (uint8_t i = 0; i < 3; i++) {
		if (ColumnMapping[colIndex][i] != BUTTON_NONE) {
			leds[ColumnMapping[colIndex][i] * 2].r = red;
			leds[ColumnMapping[colIndex][i] * 2].g = green;
			leds[ColumnMapping[colIndex][i] * 2].b = blue;
			leds[(ColumnMapping[colIndex][i] * 2) + 1].r = red;
			leds[(ColumnMapping[colIndex][i] * 2) + 1].g = green;
			leds[(ColumnMapping[colIndex][i] * 2) + 1].b = blue;
		}
	}
}

void setColumn(int colIndex, CRGB rgbColor) {
	for (uint8_t i = 0; i < COLUMN_HEIGHT; i++) {
		if (ColumnMapping[colIndex][i] != BUTTON_NONE)
			leds[ColumnMapping[colIndex][i] * 2] = rgbColor;
      leds[(ColumnMapping[colIndex][i] * 2) + 1] = rgbColor;
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

void effectWhite() {
	if (!staticOn) {
		setAll(255, 255, 255);
    FastLED.setBrightness(BRIGHTNESS);
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
	for(j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
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
		leds[BUTTON_INDEX_UP]              = CRGB::Green;
		leds[BUTTON_INDEX_UP + 1]          = CRGB::Green;
		leds[BUTTON_INDEX_K1 * 2]          = CRGB::Green;
		leds[(BUTTON_INDEX_K1 * 2) + 1]    = CRGB::Green;
		leds[BUTTON_INDEX_K2 * 2]          = CRGB::Aqua;
		leds[(BUTTON_INDEX_K2 * 2) + 1]    = CRGB::Aqua;
		leds[BUTTON_INDEX_K3 * 2]          = CRGB::Blue;
		leds[(BUTTON_INDEX_K3 * 2) + 1]    = CRGB::Blue;
		leds[BUTTON_INDEX_K4 * 2]          = CRGB::Purple;
		leds[(BUTTON_INDEX_K4 * 2) + 1]    = CRGB::Purple;
		leds[BUTTON_INDEX_P4 * 2]          = CRGB::Purple;
		leds[(BUTTON_INDEX_P4 * 2) + 1]    = CRGB::Purple;
		leds[BUTTON_INDEX_P3 * 2]          = CRGB::Blue;
		leds[(BUTTON_INDEX_P3 * 2) + 1]    = CRGB::Blue;
		leds[BUTTON_INDEX_P2 * 2]          = CRGB::Aqua;
		leds[(BUTTON_INDEX_P2 * 2) + 1]    = CRGB::Aqua;
		leds[BUTTON_INDEX_P1 * 2]          = CRGB::Green;
		leds[(BUTTON_INDEX_P1 * 2) + 1]    = CRGB::Green;
		leds[BUTTON_INDEX_RIGHT * 2]       = CRGB::Yellow;
		leds[(BUTTON_INDEX_RIGHT * 2) + 1] = CRGB::Yellow;
		leds[BUTTON_INDEX_DOWN * 2]        = CRGB::Orange;
		leds[(BUTTON_INDEX_DOWN * 2) + 1]  = CRGB::Orange;
		leds[BUTTON_INDEX_LEFT * 2]        = CRGB::Red;
		leds[(BUTTON_INDEX_LEFT * 2) + 1]  = CRGB::Red;
		show();
		staticOn = true;
	}
}
