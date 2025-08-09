#include "PeripheralMap.h"
#include "PeripheralRoles.h"
#include "HardwareInterface.h"
#include "Notes.h"

#define BUZZER_PIN 33

PeripheralMap peripheralMap;
HardwareInterface hardware(&peripheralMap);

void setup() {
    peripheralMap.addPeripheral({
        ACTUATOR_BUZZER,
        "buzzer",
        BUZZER_PIN,
        PIN_UNUSED,
        PIN_DO,
        PIN_UNUSED
    });

    hardware.configurePins();
    hardware.playSong(ACTUATOR_BUZZER, melody, beats, melodyLength, 100);
}

void loop() {
    hardware.update();
}
