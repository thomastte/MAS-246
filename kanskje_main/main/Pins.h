#ifndef PINS_H
#define PINS_H
#include <Arduino.h>

constexpr int NUM_FLOORS = 8;

extern const int led[NUM_FLOORS];
extern const int button[NUM_FLOORS];
extern const int yPin;

#endif
