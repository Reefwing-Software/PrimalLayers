// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "AutonomicController.h"

void AutonomicController::begin() {
    // Initialize setpoints, modes, etc. if needed
}

bool AutonomicController::isAutonomic(const Signal& s) const {
    switch (s.type) {
        case TEMP_HIGH:
        case BATTERY_LOW:
            return true;
        default:
            return false;
    }
}

void AutonomicController::regulate(const Signal& s) {
    switch (s.type) {
        case TEMP_HIGH:
            regulateTemperature(s.value);
            break;
        case BATTERY_LOW:
            regulateBattery(s.value);
            break;
        default:
            // Unknown autonomic signal
            break;
    }
}

void AutonomicController::regulateTemperature(float value) {
    // Example: scale fan speed or throttle system slowly
    // e.g., fanController.setPWM(value > 50 ? 255 : 128);
}

void AutonomicController::regulateBattery(float value) {
    // Example: reduce non-essential activity or dim LEDs
    // e.g., powerManager.setPowerMode(LOW_POWER);
}