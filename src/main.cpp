#include <Arduino.h>
#include <time.h>

#define DEBUG

#include "lights.h"
#include "pinout.h"
#include "power.h"
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
    // TODO buttons
    Lights::setup();
    Power::setup();

    println(F(".. Done!\nStarting IO tests..."));
    Lights::hello_world();
    println(F(".. Done!"));
}

void loop() {
    heartbeatSerial();
    PersistenceManager::loop();
    RebootManager::loop();
    Power::loop();

    // TODO remove once testing is done!
    delay(1000);
    Lights::hello_world();
}
