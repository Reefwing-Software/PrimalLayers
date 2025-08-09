// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
//
// Heartbeat LED Test Sketch

#include <Arduino.h>
#include "PeripheralMap.h"
#include "PeripheralRoles.h"
#include "HardwareInterface.h"

// Pin Definitions
#define RIGHT_LED_PIN 2
#define LEFT_LED_PIN 12

// Global objects
PeripheralMap peripheralMap;
HardwareInterface hardware(&peripheralMap);

void setup() {
    Serial.begin(115200);
    delay(1000);

    // Define LEDs as PWM outputs
    peripheralMap.addPeripheral({
        ACTUATOR_LED_LEFT,
        "led_left",
        LEFT_LED_PIN,
        PIN_UNUSED,
        PIN_PWM,
        PIN_UNUSED
    });

    peripheralMap.addPeripheral({
        ACTUATOR_LED_RIGHT,
        "led_right",
        RIGHT_LED_PIN,
        PIN_UNUSED,
        PIN_PWM,
        PIN_UNUSED
    });

    hardware.configurePins();

    // Start heartbeat effect on both LEDs
    hardware.heartbeatLED(ACTUATOR_LED_LEFT, 15);   // slower pulse
    hardware.heartbeatLED(ACTUATOR_LED_RIGHT, 5);   // faster pulse
}

void loop() {
    // Handle heartbeat animation
    hardware.update();
}
