// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "Brainstem.h"

void Brainstem::begin(NervousSystem* nervousSystem,
                      ReflexController* reflexController,
                      AutonomicController* autonomicController) {
    ns = nervousSystem;
    reflex = reflexController;
    autonomic = autonomicController;
}

void Brainstem::update() {
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
    if (abs(degrees) > 20.0) {
        ns->sendSignal({ TILT_EXCEEDED, degrees });
    }
}

void Brainstem::processCortexLevel(const Signal& s) {
    // hook for higher-level processing
}