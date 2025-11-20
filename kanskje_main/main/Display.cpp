#include "Display.h"
#include <Arduino.h>

Display::Display(int rs, int e, int d4, int d5, int d6, int d7, int backlight)
  : lcd_(rs, e, d4, d5, d6, d7), backlightPin_(backlight) {}

void Display::setup() {
    pinMode(backlightPin_, OUTPUT);
    digitalWrite(backlightPin_, HIGH);
    lcd_.begin(16, 2);
    lcd_.clear();

    // definér piler
    byte up[8] = {
      0b00100, 0b01110, 0b10101, 0b00100,
      0b00100, 0b00100, 0b00100, 0b00000
    };
    byte down[8] = {
      0b00100, 0b00100, 0b00100, 0b00100,
      0b10101, 0b01110, 0b00100, 0b00000
    };
    lcd_.createChar(0, up);
    lcd_.createChar(1, down);
}

void Display::showFloor(int floor, int spot, int line) {
    lcd_.setCursor(spot, line);
    lcd_.print(floor);
}

void Display::showDirection( Dir dir, int spot, int line) {
    lcd_.setCursor(spot, line);
    if (dir == Dir::Up) {
        lcd_.write(byte(0)); // opp-pil
    } else {
        lcd_.write(byte(1)); // ned-pil
    }
}

void Display::showIdle() {
    lcd_.setCursor(0, 1);
    lcd_.print("Idle     ");
}


void Display::showPosition(float pos, long count)
{
  lcd_.setCursor(3, 0);
  lcd_.print(pos,2);
  lcd_.print(" ");
  Serial.println(count);
  //Serial.print(", ");
}

void Display::showNothing(int spot, int line)
{
  lcd_.setCursor(spot, line);
  lcd_.print("-");
}

