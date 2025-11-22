#include "PID.h"
#include "Encoder.h"

double kp = 800;
double ki = 5;
double kd = 150;

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

  double error = in - actual;
  double output = computePID(error, dt);

  output = constrain(output, -255, 255); 
  

  delay(30);
  return output;
}

double computePID(double error, double dt) {
  double P = error;
  integral += error * dt;
  double D = (error - previousError) / dt;

  previousError = error;
  double Output = (kp * P) + (ki * integral) + (kd * D);
  return Output;
}
