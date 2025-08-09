// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "HardwareInterface.h"

HardwareInterface::HardwareInterface(PeripheralMap* map)
    : peripheralMap(map) {}

void HardwareInterface::configurePins() {
    for (PeripheralRole role = SENSOR_OBSTACLE_FRONT; role <= PERIPHERAL_UNKNOWN; role = static_cast<PeripheralRole>(role + 1)) {
        const Peripheral* p = peripheralMap->getPeripheralByRole(role);
        if (p) {
            configurePin(p->pin1, p->pin1Type);
            configurePin(p->pin2, p->pin2Type);
        }
    }
}

void HardwareInterface::configurePin(uint8_t pin, PinType type) {
    if (pin == PIN_UNUSED || type == PIN_UNUSED) return;

    switch (type) {
        case PIN_DI:
        case PIN_AI:
            pinMode(pin, INPUT);
            break;
        case PIN_DO:
        case PIN_AO:
        case PIN_PWM:
            pinMode(pin, OUTPUT);
            break;
        default:
            break;
    }
}

bool HardwareInterface::isPeripheralAvailable(PeripheralRole role) const {
    return peripheralMap->hasRole(role);
}

float HardwareInterface::readSensor(PeripheralRole role) {
    const Peripheral* p = peripheralMap->getPeripheralByRole(role);
    if (!p) return NAN;

    switch (p->pin1Type) {
        case PIN_AI:
            return analogRead(p->pin1);
        case PIN_DI:
            return digitalRead(p->pin1);
        default:
            return NAN;
    }
}

void HardwareInterface::controlActuator(PeripheralRole role, float value) {
    const Peripheral* p = peripheralMap->getPeripheralByRole(role);
    if (!p) return;

    switch (p->pin1Type) {
        case PIN_DO:
            digitalWrite(p->pin1, value > 0 ? HIGH : LOW);
            break;
        case PIN_PWM:
            analogWrite(p->pin1, (int)value);
            break;
        default:
            break;
    }
}

void HardwareInterface::blinkLED(PeripheralRole role, unsigned long durationMs) {
    const Peripheral* p = peripheralMap->getPeripheralByRole(role);
    if (!p || p->pin1Type != PIN_DO) return;

    digitalWrite(p->pin1, HIGH);

    blinkingLEDs[role] = BlinkState{
        .pin = p->pin1,
        .isBlinking = true,
        .startTime = millis(),
        .duration = durationMs
    };
}

void HardwareInterface::blinkPatternLED(PeripheralRole role, const uint16_t* pattern, size_t length) {
    const Peripheral* p = peripheralMap->getPeripheralByRole(role);
    if (!p || p->pin1Type != PIN_DO || length == 0) return;

    BlinkPatternState state;
    state.pin = p->pin1;
    state.pattern = pattern;
    state.length = length;
    state.index = 0;
    state.lastChange = millis();
    state.ledState = false;

    digitalWrite(state.pin, LOW);
    blinkPatterns[role] = state;
}

void HardwareInterface::heartbeatLED(PeripheralRole role, unsigned int rate) {
    const Peripheral* p = peripheralMap->getPeripheralByRole(role);
    if (!p || p->pin1Type != PIN_PWM) return;

    heartbeatLEDs[role] = HeartbeatState{
        .pin = p->pin1,
        .increasing = true,
        .brightness = 0,
        .lastUpdate = millis(),
        .stepDelay = rate
    };
}

void HardwareInterface::playSong(PeripheralRole role, const int* notes, const float* durations, int length, int bpm) {
    const Peripheral* p = peripheralMap->getPeripheralByRole(role);
    if (!p || p->pin1Type != PIN_DO) return;

    songPlayers[role] = SongState{
        .pin = p->pin1,
        .notes = notes,
        .durations = durations,
        .length = length,
        .index = 0,
        .bpm = bpm,
        .noteStart = millis(),
        .playing = true
    };
}

void HardwareInterface::update() {
    unsigned long now = millis();

    // Handle blinking LEDs
    for (auto it = blinkingLEDs.begin(); it != blinkingLEDs.end(); ) {
        BlinkState& blink = it->second;
        if (blink.isBlinking && (now - blink.startTime >= blink.duration)) {
            digitalWrite(blink.pin, LOW);
            it = blinkingLEDs.erase(it);
        } else {
            ++it;
        }
    }

    // Handle blink patterns
    for (auto& [role, state] : blinkPatterns) {
        if (now - state.lastChange >= state.pattern[state.index]) {
            state.ledState = !state.ledState;
            digitalWrite(state.pin, state.ledState ? HIGH : LOW);

            state.index = (state.index + 1) % state.length;
            state.lastChange = now;
        }
    }

    // Handle heartbeat LEDs
    for (auto& [role, hb] : heartbeatLEDs) {
        if (now - hb.lastUpdate >= hb.stepDelay) {
            hb.lastUpdate = now;

            // Update brightness
            if (hb.increasing) {
                hb.brightness += 5;
                if (hb.brightness >= 255) {
                    hb.brightness = 255;
                    hb.increasing = false;
                }
            } else {
                hb.brightness -= 5;
                if (hb.brightness <= 0) {
                    hb.brightness = 0;
                    hb.increasing = true;
                }
            }

            analogWrite(hb.pin, hb.brightness);
        }
    }

    // Music playback
    for (auto it = songPlayers.begin(); it != songPlayers.end(); ) {
        SongState& song = it->second;
        if (!song.playing) {
            ++it;
            continue;
        }

        float noteDurationMs = 60000.0 / song.bpm * song.durations[song.index];

        if (now - song.noteStart >= noteDurationMs) {
            noTone(song.pin); // stop previous note

            song.index++;
            if (song.index >= song.length) {
                song.playing = false;
                it = songPlayers.erase(it);
                continue;
            }

            tone(song.pin, song.notes[song.index]);
            song.noteStart = now;
        }

        ++it;
    }
}