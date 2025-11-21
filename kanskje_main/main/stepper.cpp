#include "stepper.h"
#include <ArduinoSTL.h>

void Stepper::init() {
    pinMode(aPhase, OUTPUT);
    pinMode(aEnable, OUTPUT);
    pinMode(bPhase, OUTPUT);
    pinMode(bEnable, OUTPUT);

    DDRC |= B11110000;
    cs(HIGH);
    sck(LOW);
    sdi(LOW);
    ldac(HIGH);
    setCurrent(0,0);
}

void Stepper::setCurrent(float aCurrent, float bCurrent) {
    uint16_t aVal = (aCurrent <= 1 && aCurrent > 0) ? (aCurrent * 4095) : 0;
    uint16_t bVal = (bCurrent <= 1 && bCurrent > 0) ? (bCurrent * 4095) : 0;
    aVal += 0b1011 << 12;
    bVal += 0b0011 << 12;

    // --- output A ---
    cs(LOW);
    for (uint8_t i = 15; i < 16; i--) {
        sdi((1 << i) & aVal);
        clockPulse();
    }
    cs(HIGH); 
    
    // --- output B ---
    cs(LOW);
    for (uint8_t i = 15; i < 16; i--) {
        sdi((1 << i) & bVal);
        clockPulse();
    }
    cs(HIGH);

    ldac(LOW);
    sck(HIGH);
    ldac(HIGH);
    sck(LOW);
}

void Stepper::halfStep(bool dir) {  
    dir ? (position++) : (position--);
    std::array<std::array<bool, 4>, 8> out = {
        {0, 1, 0, 1}, //A
        {0, 0, 0, 1}, //B
        {1, 1, 0, 1}, //AB
        {1, 1, 0, 0}, //A
        {1, 1, 1, 1}, //AB
        {0, 0, 1, 1}, //B
        {0, 1, 1, 1}, //AB
        {0, 1, 0, 0} //A
    };
    uint8_t state = position % 8;
    digitalWrite(aPhase, out[state][0]);
    digitalWrite(aEnable, out[state][1]);
    digitalWrite(bPhase, out[state][2]);
    digitalWrite(bEnable, out[state][3]);
};

void Stepper::doorOpen() {
    currentState = OPENING; 
};

void Stepper::doorClose() {
    currentState = CLOSING;
};

void Stepper::doorStop() {
    currentState = STOPPED; 
    //setCurrent(0,0);
}

void Stepper::update() {
    switch(currentState) {
        case OPENING:
            for (int i = 0; i < 500; i++)
            {
                halfStep(true);
                delay(3);  // sets speed of opening
            }
            break;
            
        case CLOSING:
            for (int i = 0; i < 500; i++)
            {
                halfStep(false);
                delay(3);  // sets speed of opening
            }
            break;
            
        case STOPPED:
            // Do nothing, motor is stopped
            break;
    }
}


void Stepper::setup(float aCurrent, float bCurrent) {
    // perform existing initialization and set currents
    init();
    setCurrent(aCurrent, bCurrent);
}