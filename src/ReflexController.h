// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "Messages.h"
#include "AlertnessController.h"

class ReflexController {
public:
    void begin(AlertnessController* alertController);
    bool isReflex(const Signal& s) const;
    void execute(const Signal& s) const;

private:
    AlertnessController* alertness = nullptr;

    void handleTempHigh(float value) const;
    void handleBatteryLow(float value) const;
    void handleTiltExceeded(float value) const;
};