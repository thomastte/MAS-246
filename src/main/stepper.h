#ifndef STEPPER_H
#define STEPPER_H

#include <arduino.h>
#include <ArduinoSTL.h> //comment out "const std::nothrow_t std::nothrow = { };" in "libraries\ArduinoSTL\src\new_handler.cpp"
#include <array>
#include <LiquidCrystal.h>
#include <vector>
#include <TimerOne.h>

class Stepper {
public:
    Stepper():
        aPhase{A14},
        aEnable{A15},
        bPhase{A12},
        bEnable{A13},
        position{0}
    {}
    const uint8_t aPhase;
    const uint8_t aEnable;
    const uint8_t bPhase;
    const uint8_t bEnable;
    void init();
    void setCurrent(float aCurrent, float bCurrent);
    void halfStep(bool dir);
    void doorOpen();
    void doorClose();
    void doorStop();
    void setup(float aCurrent = 0.5f, float bCurrent = 0.5f);
    void update();
    uint32_t position;

    enum State {
        STOPPED,
        OPENING,
        CLOSING
    };
    void stop();
    State getState() { return currentState; }

private:
    inline void ldac(bool val)  {(val ? (PORTC |= 0B10000000) : (PORTC &= 0B01111111));}
    inline void sdi(bool val)   {(val ? (PORTC |= 0B01000000) : (PORTC &= 0B10111111));}
    inline void sck(bool val)   {(val ? (PORTC |= 0B00100000) : (PORTC &= 0B11011111));}
    inline void cs(bool val)    {(val ? (PORTC |= 0B00010000) : (PORTC &= 0B11101111));}
    inline void clockPulse() {sck(HIGH); sck(LOW);}
    volatile State currentState;
};

#endif