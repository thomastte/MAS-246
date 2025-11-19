#include "Encoder.h"
#include "Display.h"
#include <Arduino.h>

extern Display display;

const uint8_t ENC_A = SDA;
const uint8_t ENC_B = SCL;

volatile long encoderCount = 0;
volatile uint8_t lastAB = 0;

const int8_t dirTable[16] = {
  0, -1, +1,  0,
  +1,  0,  0, -1,
  -1,  0,  0, +1,
   0, +1, -1,  0
};

void encISR() {
  uint8_t a = digitalRead(ENC_A);
  uint8_t b = digitalRead(ENC_B);
  uint8_t ab = (a << 1) | b;
  uint8_t idx = (lastAB << 2) | ab;
  encoderCount += dirTable[idx];
  lastAB = ab;
}

void Encoder::setup() {
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  lastAB = (digitalRead(ENC_A) << 1) | digitalRead(ENC_B);
  attachInterrupt(digitalPinToInterrupt(ENC_A), encISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), encISR, CHANGE);
}

void Encoder::ReadEncoder() {
  noInterrupts();
  long count = encoderCount;
  interrupts();

  float rotations = (float)count / 8384.0f;
  elevatorPosition = rotations;
  display.showPosition(rotations);
}

float Encoder::getElevatorPosition() {
  return elevatorPosition;
}
