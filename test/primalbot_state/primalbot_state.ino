// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
//
// Test the state LED blinking patterns 
// using the PrimalLayers architecture.

#include <Arduino.h>

#include "PrimalLayers.h"
#include "PeripheralMap.h"
#include "PeripheralRoles.h"
#include "HardwareInterface.h"


// Define the right LED pin
#define RIGHT_LED_PIN 12  

// Global components
PeripheralMap peripheralMap;
HardwareInterface hardware(&peripheralMap);
PrimalLayers stem;

// State cycling setup
const RobotState testStates[] = {
  STATE_IDLE,
  STATE_EXPLORING,
  STATE_AVOIDING_OBSTACLE,
  STATE_AVOIDING_EDGE,
  STATE_RECOVERING,
  STATE_ALERT,
  STATE_USER_COMMAND,
  STATE_UNKNOWN
};

const int numStates = sizeof(testStates) / sizeof(RobotState);
int currentIndex = 0;
unsigned long lastStateChange = 0;
const unsigned long stateDuration = 5000;  // ms per state

void setup() {
  Serial.begin(115200);
  delay(500);

  // Register the right LED
  peripheralMap.addPeripheral({
      ACTUATOR_LED_RIGHT,
      "led_right",
      RIGHT_LED_PIN,
      PIN_UNUSED,
      PIN_DO,
      PIN_UNUSED
  });

  hardware.configurePins();

  // Begin Brainstem with hardware access
  stem.begin(&hardware);

  // Start in the first state
  stem.setState(testStates[currentIndex]);
}

void loop() {
  unsigned long now = millis();

  hardware.update();  // Handle blinking

  // Change state every few seconds
  if (now - lastStateChange > stateDuration) {
    currentIndex = (currentIndex + 1) % numStates;
    stem.setState(testStates[currentIndex]);
    lastStateChange = now;
  }
}