// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "AlertnessController.h"
#include <Arduino.h> // for millis()

void AlertnessController::begin() {
    currentLevel = ALERT_NORMAL;
    alertnessScore = 0;
    lastStimulusTime = millis();
}

void AlertnessController::update() {
    unsigned long now = millis();
    unsigned long timeSinceLast = now - lastStimulusTime;

    // Decay alertness over time
    if (timeSinceLast > 5000) {
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

void AlertnessController::evaluateState() {
    if (alertnessScore >= 5) {
        currentLevel = ALERT_HIGH;
    } else if (alertnessScore <= -2) {
        currentLevel = ALERT_LOW;
    } else {
        currentLevel = ALERT_NORMAL;
    }

    // Clamp the score to avoid runaway values
    if (alertnessScore > 10) alertnessScore = 10;
    if (alertnessScore < -5) alertnessScore = -5;
}

AlertLevel AlertnessController::getAlertLevel() const {
    return currentLevel;
}