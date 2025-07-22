// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "PrimalLayers.h"

void PrimalLayers::begin() {
    nervousSystem.begin();
    autonomicController.begin();
    alertnessController.begin();
    reflexController.begin(&alertnessController);

    brainstem.begin(&nervousSystem, &reflexController, &autonomicController);
}

void PrimalLayers::update() {
    brainstem.update();
}