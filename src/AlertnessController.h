// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "Messages.h"

enum AlertLevel {
    ALERT_LOW,
    ALERT_NORMAL,
    ALERT_HIGH
};

class AlertnessController {
public:
    void begin();
    void update(); // Call periodically (e.g. from loop)
    void adjustBasedOn(const Signal& s);

    AlertLevel getAlertLevel() const;

private:
    AlertLevel currentLevel = ALERT_NORMAL;
    unsigned long lastStimulusTime = 0;
    int alertnessScore = 0; // crude internal state

    void evaluateState();
};