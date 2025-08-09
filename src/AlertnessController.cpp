// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "AlertnessController.h"
#include <Arduino.h> // for millis()
#include "PeripheralRoles.h"

void AlertnessController::begin(HardwareInterface* hw) {
    hardware = hw;
    currentLevel = ALERT_NORMAL;
    alertnessScore = 0;
    lastStimulusTime = millis();

    if (hardware) {
        hardware->heartbeatLED(ACTUATOR_LED_LEFT, HEARTBEAT_NORMAL);
    }
}

void AlertnessController::update() {
    unsigned long now = millis();

    // Decay alertness over time
    if (now - lastStimulusTime > 5000) {
        alertnessScore -= 1;
        evaluateState();
        lastStimulusTime = now; // decay every 5 sec
    }
}

void AlertnessController::adjustBasedOn(const Signal& s) {
    lastStimulusTime = millis();

    switch (s.type) {
        case TILT_EXCEEDED:
        case TEMP_HIGH:
            alertnessScore += 2;
            break;
        case BATTERY_LOW:
            alertnessScore += 1;
            break;
        default:
            alertnessScore += 0;
            break;
    }

    evaluateState();
}

void AlertnessController::setAlertScore(int s) {
    alertnessScore = s;
    evaluateState();
}

void AlertnessController::evaluateState() {
    // Clamp the score to avoid runaway values
    if (alertnessScore > 10) alertnessScore = 10;
    if (alertnessScore < -5) alertnessScore = -5;

    if (alertnessScore >= 5) {
        setLevel(ALERT_HIGH);
    } else if (alertnessScore <= -2) {
        setLevel(ALERT_LOW);
    } else {
        setLevel(ALERT_NORMAL);
    }
}

void AlertnessController::setLevel(AlertLevel newLevel) {
    if (newLevel == currentLevel)
        return;

    currentLevel = newLevel;

    if (!hardware) return;

    switch (newLevel) {
        case ALERT_LOW:
            hardware->heartbeatLED(ACTUATOR_LED_LEFT, HEARTBEAT_LOW);
            break;
        case ALERT_NORMAL:
            hardware->heartbeatLED(ACTUATOR_LED_LEFT, HEARTBEAT_NORMAL);
            break;
        case ALERT_HIGH:
            hardware->heartbeatLED(ACTUATOR_LED_LEFT, HEARTBEAT_HIGH);
            break;
    }
}

AlertLevel AlertnessController::getLevel() const {
    return currentLevel;
}