// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "Messages.h"

class AutonomicController {
public:
    void begin(); // optional setup if needed
    bool isAutonomic(const Signal& s) const;
    void regulate(const Signal& s);

private:
    void regulateTemperature(float value);
    void regulateBattery(float value);
    // Add more homeostatic systems as needed
};