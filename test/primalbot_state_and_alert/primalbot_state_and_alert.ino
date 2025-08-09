// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
//
// Tests Robot state on the right LED using blinkPatternLED()
// Tests Alertness level on the left LED using heartbeatLED()

#include <Arduino.h>

#include "PrimalLayers.h"
#include <PeripheralMap.h>
#include <PeripheralRoles.h>
#include <HardwareInterface.h>

// Define LED pins
#define LEFT_LED_PIN 2    // Alertness (PWM capable)
#define RIGHT_LED_PIN 12  // Robot state (DO capable)

PeripheralMap peripheralMap;
HardwareInterface hardware(&peripheralMap);
PrimalLayers stem;

RobotState testStates[] = {
  STATE_IDLE,
  STATE_EXPLORING,
  STATE_AVOIDING_OBSTACLE,
  STATE_AVOIDING_EDGE,
  STATE_RECOVERING,
  STATE_ALERT,
  STATE_USER_COMMAND,
  STATE_UNKNOWN
};

const int numStates = sizeof(testStates) / sizeof(testStates[0]);
int currentIndex = 0;
unsigned long lastStateChange = 0;
const unsigned long stateDuration = 5000; // 5 seconds

void setup() {
  Serial.begin(115200);

  // Register LEDs in PeripheralMap
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
    PIN_DO,
    PIN_UNUSED
  });

  hardware.configurePins();

  // Begin Brainstem with hardware access
  stem.begin(&hardware);
  stem.setState(testStates[currentIndex]);
  stem.setAlertScore(10); // ALERT_HIGH
}

void loop() {
  unsigned long now = millis();

  hardware.update();    // Drive both LEDs
  stem.update();        // Allow alertness to decay over time

  // Cycle robot states every few seconds
  if (now - lastStateChange > stateDuration) {
    currentIndex = (currentIndex + 1) % numStates;
    stem.setState(testStates[currentIndex]);
    lastStateChange = now;
  }
}
