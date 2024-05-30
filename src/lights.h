#pragma once

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#include "pinout.h"

/**
 * simulate using https://wokwi.com/arduino/libraries/FastLED
 *
 * References:
 * https://www.reddit.com/r/FastLED/comments/gpsc89/custom_animations/
 * https://github.com/atuline/FastLED-Demos
 * https://www.reddit.com/r/FastLED/wiki/index/user_examples
 * https://learn.adafruit.com/dotstar-belly-dance-fans/the-code
 *
 * This class should honor different timescales, but call into it periodically for proper animations
 * */

inline uint32_t pColor(uint8_t r, uint8_t g, uint8_t b) { return Adafruit_NeoPixel::Color(r, g, b); }
inline uint32_t pColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w) { return Adafruit_NeoPixel::Color(r, g, b, w); }

namespace Lights {
namespace {
const uint8_t BRIGHTNESS = 255 / 16;
const uint32_t CURRENT_LIMIT__MA = 500;
const bool COLOR_CORRECT = false;

const uint8_t N_LEDS = 5;

// https://funduino.de/nr-17-ws2812-neopixel
// https://registry.platformio.org/libraries/adafruit/Adafruit%20NeoPixel
Adafruit_NeoPixel px = Adafruit_NeoPixel(N_LEDS, Pinout::DATA_LEDS, NEO_GRBW + NEO_KHZ800);

// set all LEDs to the same color with a slight delay inbetween
// blocking; this is for debug use only!
void colorWipe(uint32_t color, uint16_t waitMs) {
    for (uint8_t i = 0; i < px.numPixels(); i++) {
        px.setPixelColor(i, color);
        px.show();
        delay(waitMs / px.numPixels());
    }
}

} // namespace

void clear() {
    px.clear();
    px.show();
}

void setup() {
    px.begin();
    // turn off asap to prevent blown fuse
    clear();
}

void hello_world() {
    const uint16_t durationMs = 500;
    colorWipe(pColor(255, 0, 0), durationMs);     // Red
    colorWipe(pColor(0, 255, 0), durationMs);     // Green
    colorWipe(pColor(0, 0, 255), durationMs);     // Blue
    colorWipe(pColor(255, 255, 255), durationMs); // Fake white
    colorWipe(pColor(0, 0, 0, 255), durationMs);  // True white
    clear();
}

void set_brightness(uint8_t brightness) {
    px.setBrightness(brightness);
    px.show();
}

void fill(uint32_t color) {
    if (COLOR_CORRECT) color = px.gamma32(color);
    Lights::px.fill(color);
    Lights::px.show();
}

} // namespace Lights