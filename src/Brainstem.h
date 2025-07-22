// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "NervousSystem.h"
#include "ReflexController.h"
#include "AutonomicController.h"

class Brainstem {
public:
    void begin(NervousSystem* nervousSystem,
               ReflexController* reflexController,
               AutonomicController* autonomicController);
    void update();

    void monitorTemperature(float tempC);
    void monitorBattery(float volts);
    void monitorTilt(float degrees);

private:
    NervousSystem* ns;
    ReflexController* reflex;
    AutonomicController* autonomic;

    void processCortexLevel(const Signal& signal);
};