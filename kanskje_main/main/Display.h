// Display.h
#ifndef DISPLAY_H
#define DISPLAY_H

#include <LiquidCrystal.h>
#include <Arduino.h>

#include "Request.h"   // for Dir enum
#include "Encoder.h"


class Display {
public:
    Display(int rs, int e, int d4, int d5, int d6, int d7, int backlight);
    void setup();                  // kjører oppstart og piler
    void showFloor(int floor, int spot, int line);     // viser nåværende etasje
    void showDirection(Dir dir,int spot,int line);   // viser pil opp/ned
    void showIdle();               // viser "Står stille"
    void showPosition(float pos);
    void showNothing(int spot, int line);
    


private:
    LiquidCrystal lcd_;
    int backlightPin_;
    byte arrowUp_[8];
    byte arrowDown_[8];
};



#endif
