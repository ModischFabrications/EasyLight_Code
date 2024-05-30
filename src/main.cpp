#include <Arduino.h>
#include <time.h>

#include "lights.h"
#include "pinout.h"
#include "power.h"
#include "buttons.h"
#include "controller.h"
#include "shared/persistence/persistenceManager.h"
#include "shared/rebootManager.h"
#include "shared/serialWrapper.h"

void setup() {
    delay(0); // reset watchdog timer (WDT)

    Pinout::setup();

    setupSerial(115200);
    println(F("Initializing..."));

    const time_t unixBuildTime = LAST_BUILD_TIME;
    print(F("Build date: "));
    printlnRaw(ctime(&unixBuildTime));  // this formatting is not great, but easy

    PersistenceManager::setup();
    RebootManager::setup();
    Buttons::setup();
    Lights::setup();
    Power::setup();
    Controller::setup();

    println(F(".. Done!\nStarting IO tests..."));
    Lights::hello_world();
    println(F(".. Done!"));
}

void loop() {
    heartbeatSerial();
    PersistenceManager::loop();
    RebootManager::loop();
    Buttons::loop();
    // Lights::loop();
    Power::loop();
    Controller::loop();

    delay(1);
    // delay(200);
}
