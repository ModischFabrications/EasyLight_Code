#pragma once

#include "lights.h"
#include "shared/serialWrapper.h"
#include <Arduino.h>

namespace State_Fire {
namespace {

const uint16_t UPDATE_DELAY = 500;

// maybe presets/ranges/matrixes are easier than scaled spread + intensity
// wait for pulsing to work before attempting anything here
uint8_t intensity = 0;
const uint8_t SPREAD = 20;

Adafruit_NeoPixel* px;

uint32_t fullRed = Adafruit_NeoPixel::ColorHSV(0, 255, 255);
uint32_t fullYellow = Adafruit_NeoPixel::ColorHSV(65535 * ((float)30 / 360), 255, 255);
// dark red, bright red, bright yellow
// maybe add some white accents?
// maybe add a off state to fade from and a midpoint?

void updateScreen() {
    for (uint8_t i = 0; i < px->numPixels(); i++) {
        // TODO pick between the reds and scale by intensity
        // TODO clamp
        // uint8_t intensity = random(intensity - SPREAD, intensity + SPREAD);
        px->setPixelColor(i, fullYellow);
    }
    px->show();
}

} // namespace

void reset() {
    // extinguish and start over
    intensity = 0;
    updateScreen();
}

void setup() { px = Lights::getPixels(); }

uint32_t nextUpdate = 0;
void loop() {
    uint32_t now = millis();
    if (now < nextUpdate) return;
    nextUpdate = now + UPDATE_DELAY;
    updateScreen();
}

void trigger() { reset(); }

} // namespace State_Fire