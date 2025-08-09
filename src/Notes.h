// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#define C4 262
#define D4 294
#define E4 330
#define F4 349
#define G4 392
#define A4 440
#define B4 494

const int melody[] = {
  C4, C4, G4, G4, A4, A4, G4,
  F4, F4, E4, E4, D4, D4, C4
};

const float beats[] = {
  1, 1, 1, 1, 1, 1, 2,
  1, 1, 1, 1, 1, 1, 2
};

const int melodyLength = sizeof(melody) / sizeof(melody[0]);