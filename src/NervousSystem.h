// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "Messages.h"

class NervousSystem {
public:
    void begin();
    void sendSignal(const Signal& signal);
    bool hasPendingSignal();
    Signal getNextSignal();

private:
    static const int MAX_QUEUE = 10;
    Signal queue[MAX_QUEUE];
    int head = 0;
    int tail = 0;
};