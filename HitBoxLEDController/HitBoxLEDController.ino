/************************************************************************************
 * HitBox RGB LED Controller
 * 
 * Some effects borrowed from:
 * https://www.tweaking4all.com/hardware/arduino/arduino-all-ledstrip-effects-in-one/
 * https://www.tweaking4all.com/forum/miscellaneous-software/lego-millenium-falcon-with-all-ledstrip-effects-in-one-fastled/
 * 
 * Adding a new effect:
 * - Create a new function with the prefix `effect`
 * - Add function call as switch case in main loop
 * - Increment the EFFECT_COUNT define
 ************************************************************************************/

// #define DEBUG
// #define USE_FEATHER

#include <FastLED.h>
#include <FlashStorage.h>

#include "ColorMappings.h"

#ifdef USE_FEATHER
// Using a Feather M0 & FeatherWing NeoPixel for debugging
#define DATA_PIN 6
#define BUTTON_PIN 9
#define BRIGHTNESS 20
#else
// Using a Seeeduino XIAO and custom WS2812 chain in the controller
#define DATA_PIN 10
#define BUTTON_PIN 9
#define BRIGHTNESS 255
#endif

// Effect defines
#define EFFECT_COUNT 30
#define CHASE_DELAY 10
#define DOT_DELAY 500

// State variables
static CRGB leds[LED_COUNT];
volatile static uint8_t selectedEffect = 0;
volatile static bool staticOn = false;
volatile static bool effectChanged = false;

// Reserve a portion of flash memory to store the selected effect (persists through power cycle, will be reset on re-flash)
FlashStorage(selectedEffectStore, uint8_t);

// ***********************
// ** Arduino Functions **
// ***********************

void setup() {
#ifdef DEBUG
	Serial.begin(115200);
	delay(500);
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
	if (effectChanged) {
		if (selectedEffect == EFFECT_COUNT - 1)
			selectedEffect = 0;
		else
			selectedEffect++;

		staticOn = false;
		selectedEffectStore.write(selectedEffect);
		effectChanged = false;
#ifdef DEBUG
		Serial.print("Changed effect to ");
		Serial.println(selectedEffect);
#endif
	}

	switch (selectedEffect) {
		case  0:  effectOff();                                            break;
		case  1:  effectRainbowChase(CHASE_DELAY);                        break;
		case  2:  effectRainbowChaseColumns(CHASE_DELAY);                 break;

		case  3:  effectApplyColorMapping(StaticRainbowButtonMapping);    break;
		case  4:  effectApplyColorMapping(NeoGeoCurvedColorMapping);      break;
		case  5:  effectApplyColorMapping(NeoGeoStraightColorMapping);    break;
		case  6:  effectApplyColorMapping(NeoGeoModernColorMapping);      break;
		case  7:  effectApplyColorMapping(SixButtonFighterColorMapping);  break;
		case  8:  effectApplyColorMapping(SixButtonPlusColorMapping);     break;
		case  9:  effectApplyColorMapping(GuiltyGearCustomColorMapping);  break;
		case 10:  effectApplyColorMapping(GuiltyGearTypeAColorMapping);   break;
		case 11:  effectApplyColorMapping(XboxColorMapping);              break;
		case 12:  effectApplyColorMapping(SuperFamicomColorMapping);      break;
		case 13:  effectApplyColorMapping(PlayStationColorMapping);       break;

		case 14:  effectStaticColor(CRGB::Red);                           break;
		case 15:  effectStaticColor(CRGB::Orange);                        break;
		case 16:  effectStaticColor(CRGB::Yellow);                        break;
		case 17:  effectStaticColor(CRGB::Green);                         break;
		case 18:  effectStaticColor(CRGB::Aqua);                          break;
		case 19:  effectStaticColor(CRGB::Blue);                          break;
		case 20:  effectStaticColor(CRGB::Purple);                        break;
		case 21:  effectStaticColor(CRGB::White);                         break;

		case 22:  effectMovingDot(0xff, 0xff, 0xff, DOT_DELAY);           break;
		case 23:  effectTwinkleRandom(5, 100, false, true);               break;

		case 24:  effectTheaterChase(0xff, 0xff, 0xff, 50);               break;
		case 25:  effectTheaterChaseRainbow(50);                          break;

		case 26:  effectColorWipe(0xff, 0xff, 0xff, 50);
		          effectColorWipe(0x00, 0x00, 0x00, 50);                  break;
		case 27:  effectRainbowWipe(50);
		          effectColorWipe(0x00, 0x00, 0x00, 50);                  break;
		
		case 28:  effectRunningLights(0xff, 0xff, 0xff, 50);              break;
		case 29:  effectRainbowRunningLights(50);                         break;
	}
}

void changeEffect() {
	if (digitalRead(BUTTON_PIN) == LOW)
		effectChanged = true;
}

// *******************
// ** LED Functions **
// *******************

void show() {
	FastLED.show();
}

void setButton(uint8_t buttonIndex, byte red, byte green, byte blue) {
	for (uint8_t i = 0; i < LEDS_PER_BUTTON; i++) {
		leds[(buttonIndex * LEDS_PER_BUTTON) + i].r = red;
		leds[(buttonIndex * LEDS_PER_BUTTON) + i].g = green;
		leds[(buttonIndex * LEDS_PER_BUTTON) + i].b = blue;
	}
}

void setButton(uint8_t buttonIndex, CRGB rgbColor) {
	for (uint8_t i = 0; i < LEDS_PER_BUTTON; i++)
		leds[(buttonIndex * LEDS_PER_BUTTON) + i] = rgbColor;
}

void setLED(uint8_t ledIndex, byte red, byte green, byte blue) {
	leds[ledIndex].r = red;
	leds[ledIndex].g = green;
	leds[ledIndex].b = blue;
}

void setLED(uint8_t ledIndex, CRGB rgbColor) {
	leds[ledIndex] = rgbColor;
}

void setColumn(uint8_t colIndex, byte red, byte green, byte blue) {
	for (uint8_t i = 0; i < COLUMN_HEIGHT; i++) {
		if (ColumnMatrix[colIndex][i] != BUTTON_NONE) {
			for (uint8_t j = 0; j < LEDS_PER_BUTTON; j++) {
				leds[(ColumnMatrix[colIndex][i] * LEDS_PER_BUTTON) + j].r = red;
				leds[(ColumnMatrix[colIndex][i] * LEDS_PER_BUTTON) + j].g = green;
				leds[(ColumnMatrix[colIndex][i] * LEDS_PER_BUTTON) + j].b = blue;
			}
		}
	}
}

void setColumn(uint8_t colIndex, CRGB rgbColor) {
	for (uint8_t i = 0; i < COLUMN_HEIGHT; i++) {
		if (ColumnMatrix[colIndex][i] != BUTTON_NONE)
			for (uint8_t j = 0; j < LEDS_PER_BUTTON; j++)
				leds[(ColumnMatrix[colIndex][i] * LEDS_PER_BUTTON) + j] = rgbColor;
	}
}

void setAll(byte red, byte green, byte blue) {
	for (uint8_t i = 0; i < LED_COUNT; i++ ) {
		setLED(i, red, green, blue);
	}
}

void setAll(CRGB rgbColor) {
	for (uint8_t i = 0; i < LED_COUNT; i++ ) {
		setLED(i, rgbColor);
	}
}

// **********************
// ** Helper Functions **
// **********************

// Used by effectRainbowCycle
byte * wheel(byte position) {
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

// **************************
// ** Effect Functions **
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

void effectColorWipe(byte red, byte green, byte blue, uint8_t delayTime) {
	for (uint16_t i = 0; i < BUTTON_COUNT; i++) {
			if (effectChanged)
				return;

			setButton(i, red, green, blue);
			show();
			delay(delayTime);
	}
}

void effectRainbowWipe(uint8_t delayTime) {
	for (uint16_t i = 0; i < BUTTON_COUNT; i++) {
			if (effectChanged)
				return;

			setButton(StaticRainbowButtonMapping[i].buttonIndex, StaticRainbowButtonMapping[i].buttonColor);
			show();
			delay(delayTime);
	}
}

void effectRainbowChase(uint8_t delayTime) {
	byte *c;
	uint16_t i, j;
	for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
		for (i = 0; i < LED_COUNT; i++) {
			if (effectChanged)
				return;

			c = wheel(((i * 256 / LED_COUNT) + j) & 255);
			setLED(i, *c, *(c + 1), *(c + 2));
		}
		show();
		delay(delayTime);
	}
}

void effectRainbowChaseColumns(uint8_t delayTime) {
	byte *c;
	uint16_t i, j;
	for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
		for (i = 0; i < COLUMN_COUNT; i++) {
			if (effectChanged)
				return;

			c = wheel(((i * 256 / COLUMN_COUNT) + j) & 255);
			setColumn(i, *c, *(c + 1), *(c + 2));
		}
		show();
		delay(delayTime);
	}
}

void effectApplyColorMapping(const ButtonColorMap colorMapping[BUTTON_COUNT]) {
	if (!staticOn) {
		for (uint8_t b = 0; b < BUTTON_COUNT; b++) {
			for (uint8_t i = 0; i < LEDS_PER_BUTTON; i++)
				leds[(colorMapping[b].buttonIndex * LEDS_PER_BUTTON) + i] = colorMapping[b].buttonColor;
		}

		show();
		staticOn = true;
	}
}

void effectMovingDot(byte red, byte green, byte blue, uint8_t delayTime) {
	setAll(CRGB::Black);
	int buttonIndex = random(BUTTON_COUNT);
	setButton(buttonIndex, red, green, blue);
	show();
	delay(delayTime);
	setButton(buttonIndex, 0, 0, 0);
}

void effectRainbowRunningLights(int delayTime) {
	for (uint8_t i = 0; i < BUTTON_COUNT * 2; i++) {
			for (uint8_t b = 0; b < BUTTON_COUNT; b++) {
				if (effectChanged)
					return;

				setButton(StaticRainbowButtonMapping[b].buttonIndex,
					((sin(StaticRainbowButtonMapping[b].buttonIndex + i) * 127 + 128) / 255) * StaticRainbowButtonMapping[b].buttonColor.r,
					((sin(StaticRainbowButtonMapping[b].buttonIndex + i) * 127 + 128) / 255) * StaticRainbowButtonMapping[b].buttonColor.g,
					((sin(StaticRainbowButtonMapping[b].buttonIndex + i) * 127 + 128) / 255) * StaticRainbowButtonMapping[b].buttonColor.b
				);
			}
			
			show();
			delay(delayTime);
	}
}

void effectRunningLights(byte red, byte green, byte blue, int delayTime) {
	for (uint8_t i = 0; i < BUTTON_COUNT * 2; i++) {
			for (uint8_t b = 0; b < BUTTON_COUNT; b++) {
				if (effectChanged)
					return;

				setButton(b,
					((sin(b + i) * 127 + 128) / 255) * red,
					((sin(b + i) * 127 + 128) / 255) * green,
					((sin(b + i) * 127 + 128) / 255) * blue
				);
			}
			
			show();
			delay(delayTime);
	}
}

void effectTwinkleRandom(uint8_t count, uint8_t delayTime, boolean onlyOne, boolean countIsMax) {
	setAll(0, 0, 0);

	uint8_t max = count;
	if (countIsMax)
		max = count;
	else
		max = random(1, count + 1);

	for (uint8_t i = 0; i < max; i++) {
		if (effectChanged)
			return;

		setButton(random(BUTTON_COUNT), random(0, 255), random(0, 255), random(0, 255));
		show();
		delay(delayTime);

		if (onlyOne)
			setAll(0, 0, 0);
	}
	
	delay(delayTime);
}

void effectTheaterChase(byte red, byte green, byte blue, uint8_t delayTime) {
	for (uint8_t j = 0; j < 10; j++) {  //do 10 cycles of chasing
		for (uint8_t q = 0; q < 3; q++) {
			for (uint8_t i = 0; i < BUTTON_COUNT; i = i + 3) {
				if (effectChanged)
					return;

				setButton(i + q, red, green, blue);    //turn every third pixel on
			}
			show();
			delay(delayTime);
		
			for (uint8_t i = 0; i < BUTTON_COUNT; i = i + 3) {
				if (effectChanged)
					return;

				setButton(i + q, 0, 0, 0);        //turn every third pixel off
			}
		}
	}
}

void effectTheaterChaseRainbow(uint8_t delayTime) {
	byte *c;
	
	for (uint8_t j = 0; j < 256; j++) {     // cycle all 256 colors in the wheel
		for (uint8_t q = 0; q < 3; q++) {
				for (uint8_t i = 0; i < BUTTON_COUNT; i = i + 3) {
					if (effectChanged)
						return;

					c = wheel((i + j) % 255);
					setButton(i + q, *c, *(c+1), *(c+2));    //turn every third pixel on
				}
				show();
				delay(delayTime);
			
				for (uint8_t i=0; i < BUTTON_COUNT; i = i + 3) {
					if (effectChanged)
						return;

					setButton(i + q, 0, 0, 0); //turn every third pixel off
				}
		}
	}
}