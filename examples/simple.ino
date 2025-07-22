// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "PrimalLayers.h"

// Instantiate the full primitive brain system
PrimalLayers primal;

void setup() {
    Serial.begin(115200);
    primal.begin();
}

void loop() {
    // Simulate sensor input (could be replaced with real sensors)
    float simulatedTempC = 47.2;     // Trigger TEMP_HIGH
    float simulatedBatteryV = 3.1;   // Trigger BATTERY_LOW
    float simulatedTiltDeg = 25.0;   // Trigger TILT_EXCEEDED

    // Forward signals to the brainstem for monitoring
    // In a real system, you'd pass actual sensor readings here
    primal.update(); // Processes any pending reflex/autonomic actions

    delay(1000); // Simulate a sensor sampling interval
}