#pragma once

#include <Arduino.h>

/**
 * Uses pins that are available both on Arduino Nano and barebones Attiny85 to simplify testing
 * https://components101.com/sites/default/files/component_pin/Arduino-Nano-Pinout.png
 * https://circuits-diy.com/wp-content/uploads/2021/11/attiny85-pinout.jpg
 * */

namespace Pinout {

// LOW is pressed, using buildin pullups
const uint8_t BUTTONS = PB3;

// low while charging, might be bypassed via LED
const uint8_t IO_CHARGING = PB4;

// clock is optional, data can also be used to PWM
const uint8_t DATA_LEDS = PB1;
const uint8_t CLOCK_LEDS = PB2;

void setup() {
    // analog does not need a pinMode
    // pinMode(BUTTONS, INPUT_PULLUP);

    pinMode(IO_CHARGING, INPUT_PULLUP);

    // managed by LEDs, but it doesn't hurt to set explicitly
    pinMode(DATA_LEDS, OUTPUT);
    pinMode(CLOCK_LEDS, OUTPUT);
}

} // namespace Pins
