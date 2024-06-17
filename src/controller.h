#pragma once

#include "modes/fire.h"
#include "modes/pulsing.h"
#include "modes/solid.h"
// ...

#include "shared/persistence/persistenceManager.h"
#include "shared/serialWrapper.h"
#include <Arduino.h>

// manages all states, they shouldn't be accessed directly!
namespace Controller {
enum State : uint8_t { OFF = 0, SOLID, PULSING, FIRE, SIZE }; // ...

typedef void (*cMethod)();
struct StateInterface {
    const State state;
    const cMethod setup;
    const cMethod loop;
    const cMethod select;
    const cMethod trigger;
};

void setState(uint8_t new_state);

namespace {
// shall have equal index to State enum
// this would be better suited for a map but arduino C++ doesn't have one
const StateInterface states[State::SIZE] = {
    {OFF, nullptr, nullptr, nullptr, nullptr},
    {SOLID, State_Solid::setup, State_Solid::loop, State_Solid::reset, State_Solid::trigger},
    {PULSING, State_Pulsing::setup, State_Pulsing::loop, State_Pulsing::reset, State_Pulsing::trigger},
    {FIRE, State_Fire::setup, State_Fire::loop, State_Fire::reset, State_Fire::trigger},
    // ...
};

const StateInterface* activeState = &states[0];

const uint8_t BrightnessSteps[] = {
    16,
    64,
    128,
    255,
};
uint8_t N_BRIGHTNESS = sizeof(BrightnessSteps) / sizeof(uint8_t);

void _toState(State new_state) {
    if (new_state == activeState->state) return;
    print(F("New State: "));

    for (const StateInterface& m : states) {
        if (m.state == new_state) {
            printlnRaw(states[new_state].state);
            activeState = &m;
            activeState->select();
            return;
        }
    }

    println(F("UNKNOWN, discarding..."));
    _toState(OFF);
}

void _toBrightness(uint8_t brightness_step) {
    print(F("New Brightness step: "));
    printRaw(brightness_step);
    Lights::set_brightness(BrightnessSteps[brightness_step]);
    print(F("->"));
    printlnRaw(BrightnessSteps[brightness_step]);
}

} // namespace

void nextBrightness() {
    Config::Configuration config = PersistenceManager::get();
    uint8_t next = config.i_brightness + 1;
    if (next >= N_BRIGHTNESS) next = 0;

    config.i_brightness = next;
    PersistenceManager::set(config);
    // display updated from config callback
}

void setState(uint8_t new_state) {
    if (new_state >= SIZE) {
        print(F("New State "));
        printRaw(new_state);
        println(F(" is out of bounds!"));
        return;
    }

    // only to prevent log spam, PM deduplicates too
    if (new_state == activeState->state) return;

    Config::Configuration config = PersistenceManager::get();
    config.i_state = new_state;
    PersistenceManager::set(config);

    // state updated from config callback
}

void nextState() {
    // TODO next + wraparound
    uint8_t next = activeState->state + 1;
    if (next >= State::SIZE) next = 0;

    Config::Configuration config = PersistenceManager::get();
    config.i_state = next;
    PersistenceManager::set(config);
}

void trigger() {
    println(F("C: Triggering state..."));
    if (activeState->trigger != nullptr) activeState->trigger();
}

void updateConfig() {
    println(F("C: Loading config..."));
    Config::Configuration config = PersistenceManager::get();

    _toState((State)config.i_state);
    _toBrightness(config.i_brightness);
}

void setup() {
    PersistenceManager::registerListener(updateConfig);

    for (const StateInterface& m : states) {
        if (m.setup == nullptr) continue;
        m.setup();
    }
}

void loop() {
    if (activeState->loop != nullptr) activeState->loop();
}

} // namespace Controller
