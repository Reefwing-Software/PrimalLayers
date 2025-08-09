// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
//
//  Test the blinkPatternLED() method in HardwareInterface.

#include <HardwareInterface.h>
#include <PeripheralMap.h>
#include <PeripheralRoles.h>

// Define LED pin
#define LEFT_LED_PIN 12

PeripheralMap peripheralMap;
HardwareInterface hardware(&peripheralMap);

const uint16_t testPattern[] = {100, 100, 400};  // Blink-blink-pause

void setup() {
  Serial.begin(115200);

  // Register the LED in the PeripheralMap
  peripheralMap.addPeripheral({
    ACTUATOR_LED_LEFT,
    "led_left",
    LEFT_LED_PIN,
    PIN_UNUSED,
    PIN_DO,
    PIN_UNUSED
  });

  // Initialize hardware
  hardware.configurePins();

  // Start blinking the LED with the test pattern
  hardware.blinkPatternLED(ACTUATOR_LED_LEFT, testPattern, sizeof(testPattern) / sizeof(uint16_t));
}

void loop() {
  // Call update to handle non-blocking blink pattern
  hardware.update();
}
