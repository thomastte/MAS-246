// ===== ENKODER OG LED-VISNING =====

// Encoderpinner (Arduino Mega)
const uint8_t ENC_A = SDA;  // pin 20
const uint8_t ENC_B = SCL;  // pin 21

// Globale variabler
volatile long encoderCount = 0;
volatile uint8_t lastAB = 0;

// Retningstabell for kvadratur
const int8_t dirTable[16] = {
  0, -1, +1,  0,
  +1,  0,  0, -1,
  -1,  0,  0, +1,
   0, +1, -1,  0
};


// ===== INTERRUPT: OPPDATER TELLER =====
void encISR() {
  uint8_t a = digitalRead(ENC_A);
  uint8_t b = digitalRead(ENC_B);
  uint8_t ab = (a << 1) | b;
  uint8_t idx = (lastAB << 2) | ab;
  encoderCount += dirTable[idx];
  lastAB = ab;
}

// LED-pinner (fra 49 til 42)
const int led[8] = {49, 48, 47, 46, 45, 44, 43, 42};


// ===== OPPSETT =====
void setup() {
  Serial.begin(9600);

  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);

  lastAB = (digitalRead(ENC_A) << 1) | digitalRead(ENC_B);

  // Sett opp LED-utganger
  for (int i = 0; i < 8; i++) {
    pinMode(led[i], OUTPUT);
    digitalWrite(led[i], LOW);
  }

  // Aktiver interrupts
  attachInterrupt(digitalPinToInterrupt(ENC_A), encISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), encISR, CHANGE);
}


// ===== HOVEDLØKKE =====
void loop() {
  noInterrupts();
  long count = encoderCount;
  interrupts();

  // Beregn antall omdreininger (1.0 = én runde)
  float rotations = (float)count / 8400.0;

  // Finn hvilken LED som skal lyse:
  // 0.00–0.49 -> LED0, 0.50–1.49 -> LED1, 1.50–2.49 -> LED2, ...
  int zone = (int)floor(rotations + 0.5);
  zone = ((zone % 8) + 8) % 8;  // sørg for 0–7 også ved negativ retning

  // Tenn bare LED for aktuell sone
  for (int i = 0; i < 8; i++) {
    digitalWrite(led[i], (i == zone) ? HIGH : LOW);
  }

  // Skriv til Serial Monitor (to desimaler)
  Serial.print("Rotasjon: ");
  Serial.print(rotations, 2);
  Serial.print(" | Aktiv LED: ");
  Serial.println(zone);

  delay(100);  // roligere oppdatering
}
