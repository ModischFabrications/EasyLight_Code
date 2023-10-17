#pragma once

#include <Arduino.h>
#include "shared/serialWrapper.h"

#ifdef __AVR__
#include <avr/wdt.h>
#endif

namespace RebootManager {
// used to prevent millis overflow on simple, reset memory fragmentation and free unused resources on more complex platforms

namespace {
// <49 days to prevent millis overflow
const uint32_t MIN_REBOOT_DELAY_MS = 24L * 60 * 60 * 1000;
const uint32_t INACTIVITY_THRESHOLD_MS = 30L * 60 * 1000;

uint32_t scheduledRestart = 0;

void setup(){
    wdt_reset();
}

void rebootNow() {
    logWarning(F("Restarting in 5s..."));

#if defined(ESP32) || defined(ESP8266)
    delay(5 * 1000);
    ESP.restart();
#elif __AVR__
    delay(1);
    // this can lead to persistent reset loops if set too low
    wdt_enable(WDTO_4S); // resets the MCU after 4 seconds
    while (true);   // let's get killed because of running in circles
#endif
}

} // namespace

void reset() { scheduledRestart = millis() + INACTIVITY_THRESHOLD_MS; }

void loop() {
    if (millis() > MIN_REBOOT_DELAY_MS && scheduledRestart < millis()) rebootNow();
}

} // namespace RebootManager
