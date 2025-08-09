// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include <Arduino.h>
#include <vector>
#include "PeripheralRoles.h"

enum PinType {
    PIN_DI,   
    PIN_DO,   
    PIN_AI,   
    PIN_AO,   
    PIN_PWM,  
    PIN_I2C,
    PIN_UART,
    PIN_UNUSED,
    PIN_UNKNOWN
};

struct Peripheral {
    PeripheralRole role;
    String name;
    uint8_t pin1;
    uint8_t pin2;
    PinType pin1Type;
    PinType pin2Type;
};

class PeripheralMap {
public:
    void addPeripheral(const Peripheral& p);
    const Peripheral* getPeripheralByRole(PeripheralRole role) const;
    const Peripheral* getPeripheralByName(const String& name) const;
    bool hasRole(PeripheralRole role) const;
    void clear();

private:
    std::vector<Peripheral> peripherals;
};