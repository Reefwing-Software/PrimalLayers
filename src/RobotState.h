// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include <Arduino.h>    // For uint16_t

enum RobotState {
    STATE_IDLE,
    STATE_EXPLORING,
    STATE_AVOIDING_OBSTACLE,
    STATE_AVOIDING_EDGE,
    STATE_RECOVERING,
    STATE_ALERT,         // temp state triggered by IR or panic
    STATE_USER_COMMAND,  // reacting to IR remote
    STATE_UNKNOWN
};

inline const char* robotStateToString(RobotState state) {
    switch (state) {
        case STATE_IDLE: return "Idle";
        case STATE_EXPLORING: return "Exploring";
        case STATE_AVOIDING_OBSTACLE: return "Avoiding Obstacle";
        case STATE_AVOIDING_EDGE: return "Avoiding Edge";
        case STATE_RECOVERING: return "Recovering";
        case STATE_ALERT: return "Alert";
        case STATE_USER_COMMAND: return "User Command";
        default: return "Unknown";
    }
}

// Blink patterns for visual feedback (in ms)
static const uint16_t PATTERN_IDLE[]               = {100, 900};
static const uint16_t PATTERN_EXPLORING[]          = {100, 100, 100, 600};
static const uint16_t PATTERN_AVOIDING_OBSTACLE[]  = {100, 100, 100, 100, 100, 500};
static const uint16_t PATTERN_AVOIDING_EDGE[]      = {300, 700};
static const uint16_t PATTERN_RECOVERING[]         = {50, 50, 50, 50, 50, 50, 500};
static const uint16_t PATTERN_USER_COMMAND[]       = {400, 100, 100, 500};
static const uint16_t PATTERN_UNKNOWN[]            = {100, 100};
static const uint16_t PATTERN_ALERT[]              = {200, 200, 200, 200, 200, 600,
                                                      600, 200, 600, 200, 600, 600,
                                                      200, 200, 200, 200, 200, 1400};