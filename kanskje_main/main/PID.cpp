#include "PID.h"
#include <Arduino.h>
#include "Encoder.h"

double kp = 0.8;
double ki = 0.20;
double kd = 0.001;

double setpoint = 0;
double lastTime = 0;
double integral = 0;
double previousError = 0;

double computePID(double error, double dt);

void PID::setup() {
 
  lastTime = millis();
}

double PID::loop(float in, float pos) {
  double now = millis();
  double dt = (now - lastTime) / 1000.0;
  lastTime = now;

  int raw = in;
  double actual = pos;

  double error = setpoint - actual;
  double output = computePID(error, dt);

  output = constrain(output, 0, 255); 
  

  //delay(30);
  return output;
}

double computePID(double error, double dt) {
  double P = error;
  integral += error * dt;
  double D = (error - previousError) / dt;

  previousError = error;
  double Output = (kp * P) + (ki * integral) + (kd * D);
  //Serial.print(Output);
  //Serial.print(", ");
  return Output;
}
