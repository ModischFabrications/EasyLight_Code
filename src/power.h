#pragma once

#include <Arduino.h>
#include <avr/power.h>

#include "pinout.h"
#include "shared/serialWrapper.h"

namespace Power {
// monitor battery/power of the system, notifying if state changed

enum BatteryState : uint8_t {
    UNDEFINED = 0,
    CHARGING,
    GOOD,
    NORMAL,
    POOR,
};
BatteryState state = UNDEFINED;

// listeners to register from outside
typedef void (*fListener)(BatteryState);

namespace {
// could be a parameter
const float ADC_MULTIPLIER = 5000 / 1024; // ADC with 10-bit resolution reading 5V

// could be a parameter
// assuming LiPo
const uint16_t BAT_POOR = 3000;
const uint16_t BAT_GOOD = 4000;
const uint16_t V_CHARGING = 4500;

const uint8_t N_MAX_LISTENERS = 7;
fListener listeners[N_MAX_LISTENERS] = {nullptr};
uint8_t i_listeners = 0;

const uint16_t UPDATE_DELAY = 30 * 1000;

uint8_t battery_pin = 0;

// notify everyone interested that a new configuration is available
void callListeners(BatteryState new_state) {
    for (uint8_t i = 0; i < i_listeners; i++) {
        fListener listener = listeners[i];
        if (listener == nullptr) {
            logError(F("Invalid listener"));
        }
        (*listener)(new_state);
    }
}

void setMode(BatteryState new_state) {
    if (new_state == state) return;
    print(F("Battery state is now "));
    printlnRaw((uint8_t)new_state);
    callListeners(new_state);
    state = new_state;
}

uint16_t readVCCExternal() {
    uint16_t adcValue = analogRead(battery_pin);
    return ADC_MULTIPLIER * adcValue;
}

// Read 1.1V reference against AVcc
// https://github.com/cano64/ArduinoSystemStatus
// http://digistump.com/wiki/digispark/quickref -> bottom
uint16_t readVCCInternal() {
// set the reference to Vcc and the measurement to the internal 1.1V reference
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
#else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif

    delay(2);            // Wait for Vref to settle
    ADCSRA |= _BV(ADSC); // Start conversion
    while (bit_is_set(ADCSRA, ADSC)); // measuring

    uint8_t low = ADCL;  // must read ADCL first - it then locks ADCH
    uint8_t high = ADCH; // unlocks both
    uint16_t reading = (high << 8) | low;

    // 1.1V is an assumption about AVcc, you will get better results with calibration
    return (1023 * 1.1 * 1000) / reading;
}

// returns millivolts
uint16_t readVCC() {
    // TODO this might be improved with averaging multiple reads in succession,
    // but that will increase delay
    if (battery_pin > 0) {
        return readVCCExternal();
    } else {
        return readVCCInternal();
    }
}

void checkPower() {
    uint16_t mV = readVCC();
    print(F("Power reading is "));
    printRaw(mV);
    println(F("mV"));

    if (mV >= V_CHARGING) {
        setMode(BatteryState::CHARGING);
    } else if (mV >= BAT_GOOD) {
        setMode(BatteryState::GOOD);
    } else if (mV >= BAT_POOR) {
        setMode(BatteryState::NORMAL);
    } else {
        setMode(BatteryState::POOR);
    }
}

} // namespace

// TODO get relative charge (0..1)

// Calls listeners automatically while loading a initial config
void registerListener(fListener listener) {
    println(F("PM: Adding listener"));

    if (i_listeners >= N_MAX_LISTENERS) {
        logError(F("PM: List is full, unable to add listener"));
        return;
    }

    listeners[i_listeners++] = listener;
}

// TODO add charge sense pin to detect when charging finished
void setup(uint8_t pinIfExternal = 0) {
    if (pinIfExternal > 0) {
        print(F("Reading battery charge from pin "));
        printlnRaw((uint16_t)pinIfExternal);
    } else {
        println(F("Reading battery charge from internal VCC ref"));
    }
    battery_pin = pinIfExternal;

    // this might not be stable yet, don't depend on it
    print(F("Initial power reading is "));
    printRaw(readVCC());
    println(F("mV"));
}

uint32_t nextUpdate = 0;
void loop() {
    uint32_t now = millis();
    if (now < nextUpdate) return;
    nextUpdate = now + UPDATE_DELAY;
    checkPower();
}

} // namespace Power
