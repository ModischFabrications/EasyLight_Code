#pragma once

#include <AceButton.h>
#include <Arduino.h>
using namespace ace_button;

#include "pinout.h"
#include "shared/serialWrapper.h"

/**
 * EasyLight uses a parallel resistor ladder, see
 * https://github.com/bxparks/AceButton/blob/develop/docs/resistor_ladder/README.md#circuit-3-parallel-resistor-ladder-recommended
 * Might want to use *Fast versions of the classes to reduce size and cycles in future
 */

namespace Buttons {

namespace {
// AceButton's checks want to be called roughly every 5ms
const uint8_t UPDATE_DELAY = 5;

// TODO cleanup static's
static const uint8_t N_BUTTONS = 4;
static AceButton B_Brightness(nullptr, 0);
AceButton B_Mode(nullptr, 1);
AceButton B_Settings(nullptr, 2);
AceButton B_None(nullptr, 3);
static AceButton* const BUTTONS[N_BUTTONS] = {
    &B_Brightness,
    &B_Mode,
    &B_Settings,
    &B_None,
};

static const uint8_t N_LEVELS = N_BUTTONS + 1;
static const uint16_t LEVELS[N_LEVELS] = {
    0,    // short to ground
    91,   // 1k
    183,  // 2.2k
    324,  // 4.7k
    1023, // open circuit
};

static LadderButtonConfig buttonConfig(Pinout::BUTTONS, N_LEVELS, LEVELS, N_BUTTONS, BUTTONS);

void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
    print(F("Buttons:handleEvent(): "));
    print(F("virtualPin: "));
    printRaw(button->getPin());
    print(F("; eventType: "));
    print(AceButton::eventName(eventType));
    print(F("; buttonState: "));
    printlnRaw(buttonState);

    // TODO actually use these infos for something useful
}

} // namespace

void setup() {
    buttonConfig.setEventHandler(handleEvent);

    // https://github.com/bxparks/AceButton/tree/develop?tab=readme-ov-file#event-types
    buttonConfig.resetFeatures();
    // kEventPressed can't be disabled and Released is on by default
    buttonConfig.setFeature(ButtonConfig::kFeatureRepeatPress);
    buttonConfig.setRepeatPressInterval(1000);
}

uint32_t nextUpdate = 0;
void loop() {
    uint32_t now = millis();
    if (now < nextUpdate) return;
    nextUpdate = now + UPDATE_DELAY;

    buttonConfig.checkButtons();

    // debug use and calibration, if needed
    // print(F("Buttons:AnalogRead(): "));
    // printlnRaw(analogRead(Pinout::BUTTONS));
}

} // namespace Buttons
