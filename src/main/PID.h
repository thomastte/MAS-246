#ifndef PID_H
#define PID_H

#include "Encoder.h"
#include <Arduino.h>
class PID
{
  public:
  void setup();
  double loop(float in, float pos);

};

#endif