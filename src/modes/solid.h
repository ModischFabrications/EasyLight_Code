#pragma once

#include "lights.h"
#include "shared/serialWrapper.h"
#include <Arduino.h>

namespace Modes_Solid {
namespace {

uint8_t i = 0;
const uint32_t Colors[] = {
    pColor(255, 0, 0),     // Red
    pColor(0, 255, 0),     // Green
    pColor(0, 0, 255),     // Blue
    pColor(255, 255, 255), // Fake white
    pColor(0, 0, 0, 255),  // True white
    // TODO actually pretty colors
};
uint8_t N_COLORS = sizeof(Colors) / sizeof(Colors[0]);

} // namespace

void reset() { Lights::fill(Colors[i]); }

void setup() {}

void loop() {}

void next() {
    if (++i >= N_COLORS) i = 0;
    reset();
}

} // namespace Modes_Solid