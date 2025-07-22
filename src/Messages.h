// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

enum SignalType {
    TEMP_HIGH,
    BATTERY_LOW,
    TILT_EXCEEDED,
    HEARTBEAT,
    STATE
};

struct Signal {
    SignalType type;
    float value; // optional payload
};