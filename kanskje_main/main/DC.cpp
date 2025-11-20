#include "DC.h"
  int enable = 7;
  int phase = 6;
  int decay = 5;  

  void DC::setup()
  {
    pinMode(enable, OUTPUT);
    pinMode(phase, OUTPUT); //direction control
    pinMode(decay, OUTPUT);

    digitalWrite(decay, LOW); //slow-decay mode
                            
    digitalWrite(phase, LOW); //direction control (HIGH=CCW LOW=CW)
    analogWrite(enable, 0); //motor speed
  }

  void DC::moveElevator(int dir, float speed)
  {
    if (dir == 1)
    {
      digitalWrite(phase, LOW);
    }
    else if (dir == -1)
    {
      digitalWrite(phase, HIGH);
    }
    analogWrite(enable, speed);
  }