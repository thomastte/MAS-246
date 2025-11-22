#ifndef ENCODER_H
#define ENCODER_H

class Encoder 
{
public:
  void setup();
  void ReadEncoder();
  float getElevatorPosition();   

private:
  float elevatorPosition = 0.0f;
};

#endif
