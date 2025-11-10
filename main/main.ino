#include <Arduino.h>
#include "stepper.h"

Stepper stepper;

void setup() {
    // Initialize stepper motor via Stepper::setup
    //Serial.begin(9600);
    stepper.setup(0.5, 0.5);
}

void loop() {
    stepper.update();

    int joyStickState = readJoystick();

    if (joyStickState == 1) {
        stepper.doorOpen();
        Serial.println("Opening Door");
    }
    else if (joyStickState == -1) {
        stepper.doorClose();
        Serial.println("Closing Door");
    }
    else if (joyStickState == 0) {
        stepper.doorStop(); 
        Serial.println("Stopping Door");
    }
    delay(1);

/*    if (Serial.available()) {
        char cmd = Serial.read();
        switch(cmd) {
            case 'o': // open
                Serial.println("Opening door");
                stepper.doorOpen();
                break;
            case 'c': // close
                Serial.println("Closing door");
                stepper.doorClose();
                break;
            case 's': // stop
                Serial.println("Stopping door");
                stepper.doorStop();
                break;
        }
    }*/
}