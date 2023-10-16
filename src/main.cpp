#include <Arduino.h>

#define DEBUG

#include "lights.h"
#include "pins.h"
#include "shared/serialWrapper.h"

void setup() {
    Pins::setup();
    setupSerial(115200);
    println(F("Starting up, this should take less than 5s..."));
    Lights::setup();
    Lights::hello_world();
}

void loop() {}
