const int ;
double motorSpeed = 3; //

double kp = 0.8;
double ki = 0.20;
double kd = 0.001;

double setpoint = 0;
double lastTime = 0;
double integral = 0;
double previousError = 0;

void PID::setup() {
  analogWrite(OUTPUT_PIN, 0);
  lastTime = millis();
}

void loop() {
  double now = millis();
  double dt = (now - lastTime) / 1000.0;
  lastTime = now;

  int raw = analogRead(INPUT_PIN);
  double actual = map(raw, 0, 1023, 0, 255);

  double error = setpoint - actual;
  double output = computePID(error, dt);

  output = constrain(output, 0, 255); 
  analogWrite(OUTPUT_PIN, output);

  delay(300);
}

double computePID(double error, double dt) {
  double P = error;
  integral += error * dt;
  double D = (error - previousError) / dt;

  previousError = error;

  return (kp * P) + (ki * integral) + (kd * D);
}
