// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "Brainstem.h"
#include "RobotState.h"

#include <math.h>    // for abs()

void Brainstem::begin(NervousSystem* nervousSystem, ReflexController* reflexController, AutonomicController* autonomicController, HardwareInterface* hardwareInterface) {
    ns = nervousSystem;
    reflex = reflexController;
    autonomic = autonomicController;
    hardware = hardwareInterface;
}

void Brainstem::update() {
    // Periodic reflex maintenance (e.g., alertness decay)
    if (reflex) {
        reflex->update();
    }

    // Process any signals in the nervous system queue
    while (ns->hasPendingSignal()) {
        Signal s = ns->getNextSignal();

        if (reflex->isReflex(s)) {
            reflex->execute(s);
        } else if (autonomic->isAutonomic(s)) {
            autonomic->regulate(s);
        } else {
            processCortexLevel(s);
        }
    }
}

void Brainstem::setState(RobotState newState) {
    if (currentState != newState) {
        currentState = newState;

        Serial.print("Robot state changed to: ");
        Serial.println(robotStateToString(newState));

        if (!hardware) return;  // fail-safe

        switch (newState) {
            case STATE_IDLE:
                hardware->blinkPatternLED(ACTUATOR_LED_RIGHT, PATTERN_IDLE, sizeof(PATTERN_IDLE) / sizeof(uint16_t));
                break;

            case STATE_EXPLORING:
                hardware->blinkPatternLED(ACTUATOR_LED_RIGHT, PATTERN_EXPLORING, sizeof(PATTERN_EXPLORING) / sizeof(uint16_t));
                break;

            case STATE_AVOIDING_OBSTACLE:
                hardware->blinkPatternLED(ACTUATOR_LED_RIGHT, PATTERN_AVOIDING_OBSTACLE, sizeof(PATTERN_AVOIDING_OBSTACLE) / sizeof(uint16_t));
                break;

            case STATE_AVOIDING_EDGE:
                hardware->blinkPatternLED(ACTUATOR_LED_RIGHT, PATTERN_AVOIDING_EDGE, sizeof(PATTERN_AVOIDING_EDGE) / sizeof(uint16_t));
                break;

            case STATE_RECOVERING:
                hardware->blinkPatternLED(ACTUATOR_LED_RIGHT, PATTERN_RECOVERING, sizeof(PATTERN_RECOVERING) / sizeof(uint16_t));
                break;

            case STATE_ALERT:
                hardware->blinkPatternLED(ACTUATOR_LED_RIGHT, PATTERN_ALERT, sizeof(PATTERN_ALERT) / sizeof(uint16_t));
                break;

            case STATE_USER_COMMAND:
                hardware->blinkPatternLED(ACTUATOR_LED_RIGHT, PATTERN_USER_COMMAND, sizeof(PATTERN_USER_COMMAND) / sizeof(uint16_t));
                break;

            case STATE_UNKNOWN:
            default:
                hardware->blinkPatternLED(ACTUATOR_LED_RIGHT, PATTERN_UNKNOWN, sizeof(PATTERN_UNKNOWN) / sizeof(uint16_t));
                break;
        }
    }
}

RobotState Brainstem::getState() const {
    return currentState;
}

void Brainstem::monitorTemperature(float tempC) {
    if (tempC > 45.0) {
        ns->sendSignal({ TEMP_HIGH, tempC });
    }
}

void Brainstem::monitorBattery(float volts) {
    if (volts < 3.3) {
        ns->sendSignal({ BATTERY_LOW, volts });
    }
}

void Brainstem::monitorTilt(float degrees) {
    if (fabs(degrees) > 20.0) {
        ns->sendSignal({ TILT_EXCEEDED, degrees });
    }
}

void Brainstem::processCortexLevel(const Signal& s) {
    // hook for higher-level processing
}