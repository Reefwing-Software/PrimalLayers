// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "Brainstem.h"
#include "NervousSystem.h"
#include "AutonomicController.h"
#include "ReflexController.h"
#include "AlertnessController.h"
#include "HardwareInterface.h"

class PrimalLayers {
public:
    void begin(HardwareInterface* hw);
    void update();

    // Wrapper methods
    void setState(RobotState state);
    void setAlertScore(int score);

private:
    Brainstem brainstem;
    NervousSystem nervousSystem;
    ReflexController reflexController;
    AutonomicController autonomicController;
    AlertnessController alertnessController;

    // The HardwareInterface object is created in the main 
    // Arduino sketch and passed into the framework. A pointer
    // is used because PrimalLayers doesn’t own the object.
    HardwareInterface* hardwareInterface = nullptr;
};