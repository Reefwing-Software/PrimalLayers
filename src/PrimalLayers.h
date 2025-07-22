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

class PrimalLayers {
public:
    void begin();
    void update();

private:
    Brainstem brainstem;
    NervousSystem nervousSystem;
    ReflexController reflexController;
    AutonomicController autonomicController;
    AlertnessController alertnessController;
};