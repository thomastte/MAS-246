#ifndef DC_H
#define DC_H
#include <Arduino.h>
#include "PID.h"
class DC
{
  public:
  void setup();
  void moveElevator(int dir, float speed);

};

#endif