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
        case TILT_EXCEEDED:
        case OBSTACLE_DETECTED:
        case EDGE_DETECTED:
        case NOISE_HIGH:
        case LIGHT_HIGH:
            return true;
        default:
            return false;
    }
}

void ReflexController::execute(const Signal& s) const {
    switch (s.type) {
        case TILT_EXCEEDED:
            handleTiltExceeded(s.value);
            break;
        case OBSTACLE_DETECTED:
            handleObstacleDetected(s.value);
            break;
        case EDGE_DETECTED:
            handleEdgeDetected(s.value);
            break;
        case NOISE_HIGH:
            handleLoudNoise(s.value);
            break;
        case LIGHT_HIGH:
            handleBrightLight(s.value);
            break;
        default:
            break;
    }
}

void ReflexController::handleTiltExceeded(float value) const {
    if (alertness && alertness->getAlertLevel() != ALERT_LOW) {
        // Example: immediately stop motors to prevent falling
        // motorController.stop();
    }
}

void ReflexController::handleObstacleDetected(float distance) const {
    if (alertness && alertness->getAlertLevel() != ALERT_LOW) {
        // Example: back up or stop immediately
        // motorController.reverse();
    }
}

void ReflexController::handleEdgeDetected(float position) const {
    if (alertness && alertness->getAlertLevel() != ALERT_LOW) {
        // Example: prevent fall by halting forward motion
        // motorController.halt();
    }
}

void ReflexController::handleLoudNoise(float decibels) const {
    if (alertness && alertness->getAlertLevel() == ALERT_HIGH) {
        // Example: enter alert posture or look toward sound
        // postureController.raiseHead();
    }
}

void ReflexController::handleBrightLight(float lux) const {
    if (alertness && alertness->getAlertLevel() != ALERT_LOW) {
        // Example: close protective shutters or shield sensors
        // visionController.dimExposure();
    }
}