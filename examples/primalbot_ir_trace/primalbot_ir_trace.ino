// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
//
// Test the three IR Sensors using the
// PrimalLayers Hardware Abstraction.

#include <Arduino.h>
#include "PeripheralMap.h"
#include "PeripheralRoles.h"
#include "HardwareInterface.h"

// IR Sensor Pin Definitions
#define IR_LEFT_PIN   35
#define IR_MIDDLE_PIN 36
#define IR_RIGHT_PIN  39

// Global Framework Components
PeripheralMap peripheralMap;
HardwareInterface hardware(&peripheralMap);

void setup() {
    Serial.begin(115200);
    delay(500);

    // Add IR sensors to the Peripheral Map
    peripheralMap.addPeripheral({
        SENSOR_IR_LEFT,
        "ir_left",
        IR_LEFT_PIN,
        PIN_UNUSED,
        PIN_AI,
        PIN_UNUSED
    });

    peripheralMap.addPeripheral({
        SENSOR_IR_MIDDLE,
        "ir_middle",
        IR_MIDDLE_PIN,
        PIN_UNUSED,
        PIN_AI,
        PIN_UNUSED
    });

    peripheralMap.addPeripheral({
        SENSOR_IR_RIGHT,
        "ir_right",
        IR_RIGHT_PIN,
        PIN_UNUSED,
        PIN_AI,
        PIN_UNUSED
    });

    hardware.configurePins();
}

void loop() {
    // Read and display IR sensor values
    int left = hardware.readSensor(SENSOR_IR_LEFT);
    int middle = hardware.readSensor(SENSOR_IR_MIDDLE);
    int right = hardware.readSensor(SENSOR_IR_RIGHT);

    Serial.print("Left: ");
    Serial.print(left);
    Serial.print(" | Middle: ");
    Serial.print(middle);
    Serial.print(" | Right: ");
    Serial.println(right);

    delay(500);
}
