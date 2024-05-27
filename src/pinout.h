#pragma once

#include <Arduino.h>

/**
 * Using pins that are available both on Arduino Nano and barebones Attiny45/85 
 * to simplify testing isn't possible (RX/TX, ...), so we remap here
 * https://components101.com/sites/default/files/component_pin/Arduino-Nano-Pinout.png
 * https://circuits-diy.com/wp-content/uploads/2021/11/attiny85-pinout.jpg
 * */

namespace Pinout {

#ifdef NANO
// parallel resistor ladder: 1k, 2.2k, 4.7k with pullup of 1k
const uint8_t BUTTONS = A0;

// low while charging, might be bypassed via LED
const uint8_t IO_CHARGING = 12;

// clock is optional, data can also be used to PWM
const uint8_t DATA_LEDS = 9;
const uint8_t CLOCK_LEDS = 10;
#elif TINY
const uint8_t BUTTONS = 3;
const uint8_t IO_CHARGING = 4;
const uint8_t DATA_LEDS = 1;
const uint8_t CLOCK_LEDS = 2;
#endif

void setup() {
    // analog does not need a pinMode, but doesn't want a pullup
    pinMode(BUTTONS, INPUT);

    pinMode(IO_CHARGING, INPUT_PULLUP);

    pinMode(DATA_LEDS, OUTPUT);
    pinMode(CLOCK_LEDS, OUTPUT);
}

} // namespace Pinout
