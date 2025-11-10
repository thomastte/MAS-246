#include <Arduino.h>
#include "stepper.h"

Stepper stepper;

void setup() {
    // Initialize stepper motor
    Serial.begin(9600);
    stepper.init();
    stepper.setCurrent(0.5,0.5);
}

void loop() {
    stepper.update();

    if (Serial.available()) {
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
    }
}