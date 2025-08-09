// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
//
// Blink AceBott's left and right blue LEDs using 
// the PrimalLayers Hardware Abstraction.

#include <Arduino.h>
#include "PeripheralMap.h"
#include "PeripheralRoles.h"
#include "HardwareInterface.h"

// Pin Definitions
#define RIGHT_LED_PIN 2
#define LEFT_LED_PIN 12

// Global instances
PeripheralMap peripheralMap;
HardwareInterface hardware(&peripheralMap);

// State for alternating
bool blinkLeft = true;
unsigned long lastBlink = 0;
const unsigned long interval = 500;         // ms
const unsigned long blinkDuration = 100;    // ms

void setup() {
    Serial.begin(115200);
    delay(1000);

    // Register both LEDs
    peripheralMap.addPeripheral({
        ACTUATOR_LED_LEFT,
        "led_left",
        LEFT_LED_PIN,
        PIN_UNUSED,
        PIN_DO,
        PIN_UNUSED
    });

    peripheralMap.addPeripheral({
        ACTUATOR_LED_RIGHT,
        "led_right",
        RIGHT_LED_PIN,
        PIN_UNUSED,
        PIN_DO,
        PIN_UNUSED
    });

    // Configure the pins
    hardware.configurePins();
}

void loop() {
    unsigned long now = millis();

    // Alternate blinking every interval
    if (now - lastBlink >= interval) {
        if (blinkLeft) {
            hardware.blinkLED(ACTUATOR_LED_LEFT, blinkDuration);
        } else {
            hardware.blinkLED(ACTUATOR_LED_RIGHT, blinkDuration);
        }

        blinkLeft = !blinkLeft;
        lastBlink = now;
    }

    // Handle non-blocking blinking
    hardware.update();
}