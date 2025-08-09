// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
//
// Test the AlertnessController heartbeat LED functionality.
// It simulates signals to increase or decrease alertness 
// and uses the left LED (ACTUATOR_LED_LEFT) to show the alert 
// level via heartbeatLED().

#include <PeripheralMap.h>
#include <PeripheralRoles.h>
#include <HardwareInterface.h>
#include <AlertnessController.h>
#include <Messages.h>

// LED pin (must be PWM-capable)
#define LEFT_LED_PIN 2

PeripheralMap peripheralMap;
HardwareInterface hardware(&peripheralMap);
AlertnessController alertness;

unsigned long lastChange = 0;
int counter = 0;

void setup() {
  Serial.begin(115200);

  // Register a PWM LED on the left
  peripheralMap.addPeripheral({
    ACTUATOR_LED_LEFT,
    "led_left",
    LEFT_LED_PIN,
    PIN_UNUSED,
    PIN_PWM,
    PIN_UNUSED
  });

  hardware.configurePins();
  alertness.begin(&hardware);  // Pass hardware into controller
}

void loop() {
  hardware.update();         // Must be called regularly
  alertness.update();        // Decay logic and heartbeat

  // Every 5 seconds, inject a test signal
  if (millis() - lastChange > 5000) {
    lastChange = millis();

    // Alternate between different test signals
    Signal s;
    switch (counter % 3) {
      case 0:
        s = { TILT_EXCEEDED, 45.0 };
        Serial.println("Simulated: TILT_EXCEEDED");
        break;
      case 1:
        s = { TEMP_HIGH, 50.0 };
        Serial.println("Simulated: TEMP_HIGH");
        break;
      case 2:
        s = { BATTERY_LOW, 3.1 };
        Serial.println("Simulated: BATTERY_LOW");
        break;
    }

    alertness.adjustBasedOn(s);
    counter++;
  }
}
