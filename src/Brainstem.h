// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "RobotState.h"

#include "NervousSystem.h"
#include "ReflexController.h"
#include "AutonomicController.h"
#include "HardwareInterface.h"

class Brainstem {
public:
    void begin(NervousSystem* nervousSystem,
               ReflexController* reflexController,
               AutonomicController* autonomicController, 
               HardwareInterface* hardwareInterface);
    void update();

    void setState(RobotState newState);
    RobotState getState() const;

    void monitorTemperature(float tempC);
    void monitorBattery(float volts);
    void monitorTilt(float degrees);

private:
    NervousSystem* ns = nullptr;
    ReflexController* reflex = nullptr;
    AutonomicController* autonomic = nullptr;
    HardwareInterface* hardware = nullptr;

    RobotState currentState = STATE_UNKNOWN;

    void processCortexLevel(const Signal& signal);
};