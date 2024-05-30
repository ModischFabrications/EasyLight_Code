#pragma once

#include "lights.h"
#include "shared/serialWrapper.h"
#include <Arduino.h>

namespace Modes_Pulsing {
namespace {

const uint16_t UPDATE_DELAY = 10;

struct PulseGroup {
    const uint32_t from_color;
    const uint32_t to_color;
    const uint16_t durationMS;
};

uint8_t i = 0;
const PulseGroup Groups[] = {
    {pColor(0, 0, 0, 100), pColor(0, 0, 0, 200), 1000},
    {pColor(16, 0, 0, 200), pColor(0, 0, 16, 200), 2000},
    {pColor(250, 0, 0), pColor(0, 0, 250), 5000},
    {pColor(0, 150, 50), pColor(0, 200, 0), 5000},
    // TODO actually pretty colors
};
uint8_t N_GROUPS = sizeof(Groups) / sizeof(Groups[0]);

void updateScreen() {
    // TODO animate, check Neopixel lib for wheel and similar mixing
    Lights::fill(Groups[i].from_color);
}

} // namespace

void reset() {
    // TODO
}

void setup() {}

uint32_t nextUpdate = 0;
void loop() {
    uint32_t now = millis();
    if (now < nextUpdate) return;
    nextUpdate = now + UPDATE_DELAY;
    updateScreen();
}

void next() {
    if (++i >= N_GROUPS) i = 0;
    reset();
}

} // namespace Modes_Pulsing