// ==== Encoder Pins (Arduino Mega) ====
const uint8_t ENC_A = SDA;  // pin 20
const uint8_t ENC_B = SCL;  // pin 21

// ==== Global Variables ====
volatile long encoderCount = 0;     // <-- must be global
volatile uint8_t lastAB = 0;

// ==== Direction Table ====
const int8_t dirTable[16] = {
  0, -1, +1,  0,
  +1,  0,  0, -1,
  -1,  0,  0, +1,
   0, +1, -1,  0
};

// ==== Interrupt Service Routine ====
void encISR() {
  uint8_t a = digitalRead(ENC_A);
  uint8_t b = digitalRead(ENC_B);
  uint8_t ab = (a << 1) | b;

  uint8_t idx = (lastAB << 2) | ab;
  encoderCount += dirTable[idx];
  lastAB = ab;
}

// ==== Setup ====
void setup() {
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);

  lastAB = (digitalRead(ENC_A) << 1) | digitalRead(ENC_B);

  Serial.begin(9600); // Match your Serial Monitor speed

  attachInterrupt(digitalPinToInterrupt(ENC_A), encISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), encISR, CHANGE);
}

// ==== Main Loop ====
void loop() {
  static long lastReported = 0;
  static unsigned long lastPrintMs = 0;

  noInterrupts();
  long count = encoderCount;  // safe copy
  interrupts();

  if (count != lastReported || millis() - lastPrintMs > 200) {
    lastPrintMs = millis();
    String dir = (count > lastReported) ? "CW" : (count < lastReported) ? "CCW" : "-";
    lastReported = count;

    // Convert to rotations and round to 2 decimals
    float rotations = (float)count / 8400.0;
    rotations = round(rotations * 100.0) / 100.0;

    Serial.print("Dir: ");
    Serial.print(dir);
    Serial.print(" | Count: ");
    Serial.print(count);
    Serial.print(" | Rotations: ");
    Serial.println(rotations, 2);
  }
}
