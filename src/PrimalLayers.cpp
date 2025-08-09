// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "PrimalLayers.h"

void PrimalLayers::begin(HardwareInterface* hw) {
    nervousSystem.begin();
    autonomicController.begin();
   
    hardwareInterface = hw;

    // Pass in hardware interface
    brainstem.begin(
        &nervousSystem,
        &reflexController,
        &autonomicController,
        hardwareInterface
    );
    
    alertnessController.begin(hardwareInterface);
    reflexController.begin(&alertnessController);
}

void PrimalLayers::update() {
    brainstem.update();
    // Future: add Control and Cognitive layers here
}

// Wrapper method for Brainstem state management
void PrimalLayers::setState(RobotState state) {
    brainstem.setState(state);
}

void PrimalLayers::setAlertScore(int score) {
    alertnessController.setAlertScore(score);
}