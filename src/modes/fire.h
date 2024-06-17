#pragma once

#include "lights.h"
#include "shared/serialWrapper.h"
#include <Arduino.h>

namespace State_Fire {
namespace {

const uint16_t UPDATE_DELAY = 500;

void updateScreen() {
    // TODO
    Lights::colorWipe(pColor(200, 0, 0), 240);
    Lights::colorWipe(pColor(255, 0, 0), 240);
}

} // namespace

void reset() {
    // TODO extinguish and start over
}

void setup() {}

uint32_t nextUpdate = 0;
void loop() {
    uint32_t now = millis();
    if (now < nextUpdate) return;
    nextUpdate = now + UPDATE_DELAY;
    updateScreen();
}

void trigger() { reset(); }

} // namespace State_Fire