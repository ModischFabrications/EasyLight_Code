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
    // TODO buttons
    Lights::setup();
    Power::setup();

    println(F(".. Done!"));
    Lights::hello_world();
}

void loop() {
    heartbeatSerial();
    PersistenceManager::loop();
    RebootManager::loop();
    Power::loop();
    delay(1);
}
