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
    uint8_t i_mode;
    uint8_t i_param;

    const bool operator==(const Configuration& other) {
        return (this->i_mode == other.i_mode && this->i_brightness == other.i_brightness &&
                this->i_param == other.i_param);
    }
};

const Configuration defaultConfiguration = {0, 255 / 16};

} // namespace Config