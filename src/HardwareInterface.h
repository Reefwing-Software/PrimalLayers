// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include <Arduino.h>
#include <map>
#include "PeripheralMap.h"

class HardwareInterface {
public:
    HardwareInterface(PeripheralMap* map);

    void configurePins();
    float readSensor(PeripheralRole role);
    void controlActuator(PeripheralRole role, float value);
    bool isPeripheralAvailable(PeripheralRole role) const;

    void blinkLED(PeripheralRole role, unsigned long durationMs = 100);
    void blinkPatternLED(PeripheralRole role, const uint16_t* pattern, size_t length);
    void heartbeatLED(PeripheralRole role, unsigned int rate = 5); // lower is faster
    void playSong(PeripheralRole role, const int* notes, const float* durations, int length, int bpm = 120);

    void update(); // call this regularly in the main loop

private:
    PeripheralMap* peripheralMap;

    void configurePin(uint8_t pin, PinType type);

    struct BlinkState {
        uint8_t pin;
        bool isBlinking = false;
        unsigned long startTime = 0;
        unsigned long duration = 0;
    };

    struct BlinkPatternState {
        uint8_t pin;
        const uint16_t* pattern;     // sequence of on/off durations in ms
        size_t length;
        size_t index = 0;
        unsigned long lastChange = 0;
        bool ledState = false;
    };

    struct HeartbeatState {
        uint8_t pin;
        bool increasing = true;
        uint8_t brightness = 0;
        unsigned long lastUpdate = 0;
        unsigned int stepDelay = 5; // time between brightness steps
    };

    struct SongState {
        uint8_t pin;
        const int* notes;
        const float* durations;
        int length;
        int index = 0;
        int bpm;
        unsigned long noteStart = 0;
        bool playing = false;
    };

    std::map<PeripheralRole, BlinkState> blinkingLEDs;
    std::map<PeripheralRole, BlinkPatternState> blinkPatterns;
    std::map<PeripheralRole, HeartbeatState> heartbeatLEDs;
    std::map<PeripheralRole, SongState> songPlayers;
};