#pragma once

#include <stdint.h>

namespace Config {
namespace {
/**
 * change with each design iteration to prevent EEPROM inconsistency and help
 * with wear leveling of EEPROM cells.
 * */
const uint8_t VERSION = 1;
} // namespace

struct Configuration {
    uint8_t i_brightness;
    uint8_t i_state;
    // uint8_t i_param;

    const bool operator==(const Configuration& other) {
        return (this->i_brightness == other.i_brightness && this->i_state == other.i_state);
    }
};

const Configuration defaultConfiguration = {0, 0};

} // namespace Config