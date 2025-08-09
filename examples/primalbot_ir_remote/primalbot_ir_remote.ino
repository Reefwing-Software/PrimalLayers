// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
//
// IR Remote Sensor Test using the
// PrimalLayers Hardware Abstraction.

#include <Arduino.h>
#include<IRremote.h>

#include "PeripheralMap.h"
#include "PeripheralRoles.h"
#include "HardwareInterface.h"

// IR Receiver pin
#define IR_REMOTE_PIN 4

// Framework components
PeripheralMap peripheralMap;
HardwareInterface hardware(&peripheralMap);

// IRremote instance
IRrecv irRemote(IR_REMOTE_PIN);

void setup() {
    Serial.begin(115200);
    delay(500);

    // Register the IR receiver in the PeripheralMap
    peripheralMap.addPeripheral({
        SENSOR_IR_REMOTE,
        "ir_remote",
        IR_REMOTE_PIN,
        PIN_UNUSED,
        PIN_DI,
        PIN_UNUSED
    });

    hardware.configurePins();

    // Start IR remote receiving
    irRemote.enableIRIn();
}

void loop() {
    hardware.update(); // optional if using other systems

    if (irRemote.decode()) {
        Serial.print("Received HEX: ");
        Serial.print(irRemote.decodedIRData.decodedRawData, HEX);
        Serial.print(", Command 0x");
        Serial.println(irRemote.decodedIRData.command, HEX);

        // Debug - if unsure of protocol
        // irRemote.printIRResultShort(&Serial);

        irRemote.resume();  // Ready for next value
    }

    delay(100); // small delay to avoid spamming
}