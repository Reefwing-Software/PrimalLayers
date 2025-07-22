// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "NervousSystem.h"

void NervousSystem::begin() {
    head = 0;
    tail = 0;
}

void NervousSystem::sendSignal(const Signal& signal) {
    int next = (tail + 1) % MAX_QUEUE;
    if (next != head) { // queue not full
        queue[tail] = signal;
        tail = next;
    }
}

bool NervousSystem::hasPendingSignal() {
    return head != tail;
}

Signal NervousSystem::getNextSignal() {
    Signal signal = queue[head];
    head = (head + 1) % MAX_QUEUE;
    return signal;
}