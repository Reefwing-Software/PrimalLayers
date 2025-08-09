// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "PeripheralMap.h"

void PeripheralMap::addPeripheral(const Peripheral& p) {
    peripherals.push_back(p);
}

const Peripheral* PeripheralMap::getPeripheralByRole(PeripheralRole role) const {
    for (const auto& p : peripherals) {
        if (p.role == role) {
            return &p;
        }
    }
    return nullptr;
}

const Peripheral* PeripheralMap::getPeripheralByName(const String& name) const {
    for (const auto& p : peripherals) {
        if (p.name == name) {
            return &p;
        }
    }
    return nullptr;
}

bool PeripheralMap::hasRole(PeripheralRole role) const {
    return getPeripheralByRole(role) != nullptr;
}

void PeripheralMap::clear() {
    peripherals.clear();
}