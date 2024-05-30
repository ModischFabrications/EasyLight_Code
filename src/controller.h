#pragma once

#include "modes/fire.h"
#include "modes/pulsing.h"
#include "modes/solid.h"
// ...

#include "shared/persistence/persistenceManager.h"
#include "shared/serialWrapper.h"
#include <Arduino.h>

// manages all modes, they shouldn't be accessed directly!
namespace Controller {
enum Mode : uint8_t { OFF = 0, SOLID, PULSING, FIRE, SIZE }; // ...

typedef void (*cMethod)();
struct ModeInterface {
    const Mode mode;
    const cMethod setup;
    const cMethod loop;
    const cMethod select;
    const cMethod next;
};

void setMode(uint8_t new_mode);

namespace {
// shall have equal index to Mode enum
// this would be better suited for a map but arduino C++ doesn't have one
const ModeInterface modes[Mode::SIZE] = {
    {OFF, nullptr, nullptr, nullptr, nullptr},
    {SOLID, Modes_Solid::setup, Modes_Solid::loop, Modes_Solid::reset, Modes_Solid::next},
    {PULSING, Modes_Pulsing::setup, Modes_Pulsing::loop, Modes_Pulsing::reset, Modes_Pulsing::next},
    {FIRE, Modes_Fire::setup, Modes_Fire::loop, Modes_Fire::reset, Modes_Fire::next},
    // ...
};

const ModeInterface* activeMode = &modes[0];

const uint8_t BrightnessSteps[] = {
    8, 16, 64, 128, 255,
};
uint8_t N_BRIGHTNESS = sizeof(BrightnessSteps) / sizeof(uint8_t);

void _toMode(Mode new_mode) {
    if (new_mode == activeMode->mode) return;
    print(F("New Mode: "));

    for (const ModeInterface& m : modes) {
        if (m.mode == new_mode) {
            printlnRaw(modes[new_mode].mode);
            activeMode = &m;
            activeMode->select();
            return;
        }
    }

    println(F("UNKNOWN, discarding..."));
    _toMode(OFF);
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

void setMode(uint8_t new_mode) {
    if (new_mode >= SIZE) {
        print(F("New Mode "));
        printRaw(new_mode);
        println(F(" is out of bounds!"));
        return;
    }

    // only to prevent log spam, PM deduplicates too
    if (new_mode == activeMode->mode) return;

    Config::Configuration config = PersistenceManager::get();
    config.i_mode = new_mode;
    PersistenceManager::set(config);

    // mode updated from config callback
}

void nextMode() {
    // TODO next + wraparound
    uint8_t next = activeMode->mode + 1;
    if (next >= Mode::SIZE) next = 0;

    Config::Configuration config = PersistenceManager::get();
    config.i_mode = next;
    PersistenceManager::set(config);
}

void updateConfig() {
    println(F("C: Loading config..."));
    Config::Configuration config = PersistenceManager::get();

    _toMode((Mode)config.i_mode);
    _toBrightness(config.i_brightness);
}

void setup() {
    PersistenceManager::registerListener(updateConfig);

    for (const ModeInterface& m : modes) {
        if (m.setup == nullptr) continue;
        m.setup();
    }
}

void loop() {
    if (activeMode->loop != nullptr) activeMode->loop();
}

} // namespace Controller
