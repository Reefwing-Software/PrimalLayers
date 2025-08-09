// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "Messages.h"
#include "HardwareInterface.h"

enum AlertLevel {
    ALERT_LOW,
    ALERT_NORMAL,
    ALERT_HIGH
};

// Heartbeat rate (delay between brightness changes)
static const unsigned int HEARTBEAT_LOW = 40;     // slower pulse
static const unsigned int HEARTBEAT_NORMAL = 20;  // medium
static const unsigned int HEARTBEAT_HIGH = 5;     // rapid pulse

class AlertnessController {
public:
    void begin(HardwareInterface* hw);
    void update(); 
    void adjustBasedOn(const Signal& s);

    AlertLevel getLevel() const;
    void setAlertScore(int s);

private:
    AlertLevel currentLevel = ALERT_NORMAL;
    unsigned long lastStimulusTime = 0;
    int alertnessScore = 0; 

    HardwareInterface* hardware = nullptr;

    void evaluateState();
    void setLevel(AlertLevel newLevel);
};