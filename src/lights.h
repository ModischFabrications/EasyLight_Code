#pragma once

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#include "pins.h"

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

namespace Lights {
namespace {
const uint8_t BRIGHTNESS = 127;
const uint32_t CURRENT_LIMIT__MA = 500;

const uint8_t N_LEDS = 5;

const float SMOOTHING = 1.5;

// https://funduino.de/nr-17-ws2812-neopixel
// https://registry.platformio.org/libraries/adafruit/Adafruit%20NeoPixel
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(N_LEDS, Pins::DATA_LEDS, NEO_GRBW + NEO_KHZ800);

void clear() {
    pixels.clear();
    pixels.show();
}

void colorWipe(uint32_t color, int wait) {
    for (uint8_t i = 0; i < pixels.numPixels(); i++) { // For each pixel in strip...
        pixels.setPixelColor(i, color);                //  Set pixel's color (in RAM)
        pixels.show();                                 //  Update strip to match
        delay(wait);                                   //  Pause for a moment
    }
}

} // namespace

void turn_off() {
    Serial.println(F("turning off"));
    clear();
}

void setup() {
    pixels.begin();
    pixels.setBrightness(BRIGHTNESS);
    clear();
}

void hello_world() {
    colorWipe(pixels.Color(255, 0, 0), 100);    // Red
    colorWipe(pixels.Color(0, 255, 0), 100);    // Green
    colorWipe(pixels.Color(0, 0, 255), 100);    // Blue
    colorWipe(pixels.Color(0, 0, 0, 255), 100); // True white (not RGB white)
    clear();
}

} // namespace Lights