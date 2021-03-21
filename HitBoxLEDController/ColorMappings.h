#ifndef _COLOR_MAPPINGS_
#define _COLOR_MAPPINGS_

#include <FastLED.h>
#include "ButtonDefinitions.h"

struct ButtonColorMap {
	ButtonColorMap(uint8_t bi, CRGB bc)
		: buttonIndex(bi), buttonColor(bc) { }
	uint8_t buttonIndex;
	CRGB buttonColor;
};

const ButtonColorMap StaticRainbowButtonMapping[BUTTON_COUNT] = {
	ButtonColorMap(BUTTON_INDEX_UP, CRGB::Green),
	ButtonColorMap(BUTTON_INDEX_K1, CRGB::Green),
	ButtonColorMap(BUTTON_INDEX_K2, CRGB::Aqua),
	ButtonColorMap(BUTTON_INDEX_K3, CRGB::Blue),
	ButtonColorMap(BUTTON_INDEX_K4, CRGB::Purple),
	ButtonColorMap(BUTTON_INDEX_P4, CRGB::Purple),
	ButtonColorMap(BUTTON_INDEX_P3, CRGB::Blue),
	ButtonColorMap(BUTTON_INDEX_P2, CRGB::Aqua),
	ButtonColorMap(BUTTON_INDEX_P1, CRGB::Green),
	ButtonColorMap(BUTTON_INDEX_RIGHT, CRGB::Yellow),
	ButtonColorMap(BUTTON_INDEX_DOWN, CRGB::Orange),
	ButtonColorMap(BUTTON_INDEX_LEFT, CRGB::Red),
};

const ButtonColorMap GuiltyGearCustomColorMapping[BUTTON_COUNT] = {
	ButtonColorMap(BUTTON_INDEX_UP, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_K1, CRGB::Blue),     // K
	ButtonColorMap(BUTTON_INDEX_K2, CRGB::Orange),   // D
	ButtonColorMap(BUTTON_INDEX_K3, CRGB::Black),
	ButtonColorMap(BUTTON_INDEX_K4, CRGB::Black),
	ButtonColorMap(BUTTON_INDEX_P4, CRGB::Black),
	ButtonColorMap(BUTTON_INDEX_P3, CRGB::Red),      // HS
	ButtonColorMap(BUTTON_INDEX_P2, CRGB::Green),    // S
	ButtonColorMap(BUTTON_INDEX_P1, CRGB::Purple),   // P
	ButtonColorMap(BUTTON_INDEX_RIGHT, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_DOWN, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_LEFT, CRGB::White),
};

const ButtonColorMap GuiltyGearTypeAColorMapping[BUTTON_COUNT] = {
	ButtonColorMap(BUTTON_INDEX_UP, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_K1, CRGB::Purple),   // P
	ButtonColorMap(BUTTON_INDEX_K2, CRGB::Black),
	ButtonColorMap(BUTTON_INDEX_K3, CRGB::Orange),   // D
	ButtonColorMap(BUTTON_INDEX_K4, CRGB::Black),
	ButtonColorMap(BUTTON_INDEX_P4, CRGB::Black),
	ButtonColorMap(BUTTON_INDEX_P3, CRGB::Red),      // HS
	ButtonColorMap(BUTTON_INDEX_P2, CRGB::Green),    // S
	ButtonColorMap(BUTTON_INDEX_P1, CRGB::Blue),     // K
	ButtonColorMap(BUTTON_INDEX_RIGHT, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_DOWN, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_LEFT, CRGB::White),
};

const ButtonColorMap NeoGeoCurvedColorMapping[BUTTON_COUNT] = {
	ButtonColorMap(BUTTON_INDEX_UP, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_K1, CRGB::Red),      // A
	ButtonColorMap(BUTTON_INDEX_K2, CRGB::Black),
	ButtonColorMap(BUTTON_INDEX_K3, CRGB::Black),
	ButtonColorMap(BUTTON_INDEX_K4, CRGB::Black),
	ButtonColorMap(BUTTON_INDEX_P4, CRGB::Black),
	ButtonColorMap(BUTTON_INDEX_P3, CRGB::Blue),     // D
	ButtonColorMap(BUTTON_INDEX_P2, CRGB::Green),    // C
	ButtonColorMap(BUTTON_INDEX_P1, CRGB::Yellow),   // B
	ButtonColorMap(BUTTON_INDEX_RIGHT, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_DOWN, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_LEFT, CRGB::White),
};

const ButtonColorMap NeoGeoModernColorMapping[BUTTON_COUNT] = {
	ButtonColorMap(BUTTON_INDEX_UP, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_K1, CRGB::Yellow),   // B
	ButtonColorMap(BUTTON_INDEX_K2, CRGB::Blue),     // D
	ButtonColorMap(BUTTON_INDEX_K3, CRGB::Black),
	ButtonColorMap(BUTTON_INDEX_K4, CRGB::Black),
	ButtonColorMap(BUTTON_INDEX_P4, CRGB::Black),
	ButtonColorMap(BUTTON_INDEX_P3, CRGB::Black),
	ButtonColorMap(BUTTON_INDEX_P2, CRGB::Green),    // C
	ButtonColorMap(BUTTON_INDEX_P1, CRGB::Red),      // A
	ButtonColorMap(BUTTON_INDEX_RIGHT, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_DOWN, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_LEFT, CRGB::White),
};

const ButtonColorMap NeoGeoStraightColorMapping[BUTTON_COUNT] = {
	ButtonColorMap(BUTTON_INDEX_UP, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_K1, CRGB::Black),
	ButtonColorMap(BUTTON_INDEX_K2, CRGB::Black),
	ButtonColorMap(BUTTON_INDEX_K3, CRGB::Black),
	ButtonColorMap(BUTTON_INDEX_K4, CRGB::Black),
	ButtonColorMap(BUTTON_INDEX_P4, CRGB::Blue),     // D
	ButtonColorMap(BUTTON_INDEX_P3, CRGB::Green),    // C
	ButtonColorMap(BUTTON_INDEX_P2, CRGB::Yellow),   // B
	ButtonColorMap(BUTTON_INDEX_P1, CRGB::Red),      // A
	ButtonColorMap(BUTTON_INDEX_RIGHT, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_DOWN, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_LEFT, CRGB::White),
};

const ButtonColorMap PlayStationColorMapping[BUTTON_COUNT] = {
	ButtonColorMap(BUTTON_INDEX_UP, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_K1, CRGB::Blue),     // Cross
	ButtonColorMap(BUTTON_INDEX_K2, CRGB::Red),      // Circle
	ButtonColorMap(BUTTON_INDEX_K3, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_K4, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_P4, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_P3, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_P2, CRGB::Green),    // Triangle
	ButtonColorMap(BUTTON_INDEX_P1, CRGB::Purple),   // Square
	ButtonColorMap(BUTTON_INDEX_RIGHT, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_DOWN, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_LEFT, CRGB::White),
};

const ButtonColorMap SixButtonFighterColorMapping[BUTTON_COUNT] = {
	ButtonColorMap(BUTTON_INDEX_UP, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_K1, CRGB::Blue),     // LK
	ButtonColorMap(BUTTON_INDEX_K2, CRGB::Yellow),   // MK
	ButtonColorMap(BUTTON_INDEX_K3, CRGB::Red),      // HK
	ButtonColorMap(BUTTON_INDEX_K4, CRGB::Black),
	ButtonColorMap(BUTTON_INDEX_P4, CRGB::Black),
	ButtonColorMap(BUTTON_INDEX_P3, CRGB::Red),      // HP
	ButtonColorMap(BUTTON_INDEX_P2, CRGB::Yellow),   // MP
	ButtonColorMap(BUTTON_INDEX_P1, CRGB::Blue),     // LP
	ButtonColorMap(BUTTON_INDEX_RIGHT, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_DOWN, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_LEFT, CRGB::White),
};

const ButtonColorMap SixButtonPlusColorMapping[BUTTON_COUNT] = {
	ButtonColorMap(BUTTON_INDEX_UP, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_K1, CRGB::Blue),     // LK
	ButtonColorMap(BUTTON_INDEX_K2, CRGB::Yellow),   // MK
	ButtonColorMap(BUTTON_INDEX_K3, CRGB::Red),      // HK
	ButtonColorMap(BUTTON_INDEX_K4, CRGB::Purple),   // M1
	ButtonColorMap(BUTTON_INDEX_P4, CRGB::Purple),   // M2
	ButtonColorMap(BUTTON_INDEX_P3, CRGB::Red),      // HP
	ButtonColorMap(BUTTON_INDEX_P2, CRGB::Yellow),   // MP
	ButtonColorMap(BUTTON_INDEX_P1, CRGB::Blue),     // LP
	ButtonColorMap(BUTTON_INDEX_RIGHT, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_DOWN, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_LEFT, CRGB::White),
};

const ButtonColorMap SuperFamicomColorMapping[BUTTON_COUNT] = {
	ButtonColorMap(BUTTON_INDEX_UP, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_K1, CRGB::Yellow),   // B
	ButtonColorMap(BUTTON_INDEX_K2, CRGB::Red),      // A
	ButtonColorMap(BUTTON_INDEX_K3, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_K4, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_P4, CRGB::White),    // LB
	ButtonColorMap(BUTTON_INDEX_P3, CRGB::White),    // RB
	ButtonColorMap(BUTTON_INDEX_P2, CRGB::Blue),     // X
	ButtonColorMap(BUTTON_INDEX_P1, CRGB::Green),    // Y
	ButtonColorMap(BUTTON_INDEX_RIGHT, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_DOWN, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_LEFT, CRGB::White),
};

const ButtonColorMap XboxColorMapping[BUTTON_COUNT] = {
	ButtonColorMap(BUTTON_INDEX_UP, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_K1, CRGB::Green),    // A
	ButtonColorMap(BUTTON_INDEX_K2, CRGB::Red),      // B
	ButtonColorMap(BUTTON_INDEX_K3, CRGB::White),    // RT
	ButtonColorMap(BUTTON_INDEX_K4, CRGB::White),    // LT
	ButtonColorMap(BUTTON_INDEX_P4, CRGB::White),    // LB
	ButtonColorMap(BUTTON_INDEX_P3, CRGB::White),    // RB
	ButtonColorMap(BUTTON_INDEX_P2, CRGB::Yellow),   // Y
	ButtonColorMap(BUTTON_INDEX_P1, CRGB::Blue),     // X
	ButtonColorMap(BUTTON_INDEX_RIGHT, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_DOWN, CRGB::White),
	ButtonColorMap(BUTTON_INDEX_LEFT, CRGB::White),
};

#endif
