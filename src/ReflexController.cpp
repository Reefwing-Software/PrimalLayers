// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "ReflexController.h"

void ReflexController::begin(AlertnessController* alertController) {
    alertness = alertController;
}

bool ReflexController::isReflex(const Signal& s) const {
    switch (s.type) {
        case TEMP_HIGH:
        case BATTERY_LOW:
        case TILT_EXCEEDED:
            return true;
        default:
            return false;
    }
}

void ReflexController::execute(const Signal& s) const {
    switch (s.type) {
        case TEMP_HIGH:
            handleTempHigh(s.value);
            break;
        case BATTERY_LOW:
            handleBatteryLow(s.value);
            break;
        case TILT_EXCEEDED:
            handleTiltExceeded(s.value);
            break;
        default:
            break;
    }
}

void ReflexController::handleTempHigh(float value) const {
    if (alertness && alertness->getAlertLevel() != ALERT_LOW) {
        // Trigger active cooling or limit performance
        // Example: fanController.setSpeed(HIGH);
    } else {
        // Reflex suppressed due to low alertness
    }
}

void ReflexController::handleBatteryLow(float value) const {
    if (alertness && alertness->getAlertLevel() == ALERT_HIGH) {
        // Immediate aggressive power shedding
        // Example: powerManager.enterEmergencyMode();
    } else {
        // Standard power-saving response or defer to autonomic
    }
}

void ReflexController::handleTiltExceeded(float value) const {
    if (alertness && alertness->getAlertLevel() != ALERT_LOW) {
        // Stop motors, rebalance
        // Example: motorController.stop();
    } else {
        // Reflex ignored or delayed due to inattentive state
    }
}