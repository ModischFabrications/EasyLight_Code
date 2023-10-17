#include <Arduino.h>

#define DEBUG

#include "lights.h"
#include "pinout.h"
#include "power.h"
#include "shared/persistence/persistenceManager.h"
#include "shared/rebootManager.h"
#include "shared/serialWrapper.h"

void setup() {
    Pinout::setup();

    setupSerial(115200);
    println(F("Starting up, this should take less than 5s..."));

    PersistenceManager::setup();
    RebootManager::setup();
    Power::setup();
    // buttons -> https://github.com/bxparks/AceButton/blob/develop/docs/resistor_ladder/README.md
    // https://github.com/ModischFabrications/EasyLight_PCB#button-inputs
    Lights::setup();

    println(F(".. Done!"));
    Lights::hello_world();
}

void loop() {
    heartbeatSerial();
    PersistenceManager::loop();
    RebootManager::loop();
    Power::loop();
}
