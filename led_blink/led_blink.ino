// ====== Multi-button toggle-blink for 8 LEDs ======
// Assumes buttons use external pulldowns (press = HIGH).
// If you use INPUT_PULLUP wiring (press = LOW), see note below.

#include <Arduino.h>

const uint8_t ledPins[8]    = {49, 48, 47, 46, 45, 44, 43, 42}; // LED-0 .. LED-7
const uint8_t buttonPins[8] = {22, 23, 24, 25, 26, 27, 28, 29}; // BTN-0 .. BTN-7

// Per-channel state
struct Chan {
  bool blinking = false;             // is this LED currently blinking?
  bool ledState = LOW;               // current LED output state
  bool lastButtonStable = LOW;       // last *debounced* reading
  bool lastButtonRaw = LOW;          // last raw read for debounce tracking
  unsigned long lastDebounce = 0;    // debounce timer
  unsigned long lastBlink = 0;       // blink timer
  uint16_t intervalMs = 300;         // blink speed (change per channel if you want)
};

Chan ch[8];

const unsigned long debounceMs = 0; // 30–50 ms is typical

void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);

    pinMode(buttonPins[i], INPUT);   // external pulldown expected
    ch[i].blinking = false;
    ch[i].ledState = LOW;
  }
}

void handleButton(int i) {
  bool raw = digitalRead(buttonPins[i]);

  if (raw != ch[i].lastButtonRaw) {            // input changed: reset debounce timer
    ch[i].lastDebounce = millis();
    ch[i].lastButtonRaw = raw;
  }

  if (millis() - ch[i].lastDebounce >= debounceMs) {
    // reading is stable; check for a rising edge (LOW -> HIGH)
    if (raw != ch[i].lastButtonStable) {
      ch[i].lastButtonStable = raw;
      if (raw == HIGH) {
        ch[i].blinking = !ch[i].blinking;      // toggle blinking on press
        if (!ch[i].blinking) {
          ch[i].ledState = LOW;                // ensure off when stopping
          digitalWrite(ledPins[i], LOW);
        }
        // optional: reset blink timer so it starts clean
        ch[i].lastBlink = millis();
      }
    }
  }
}

void handleBlink(int i) {
  if (!ch[i].blinking) return;

  unsigned long now = millis();
  if (now - ch[i].lastBlink >= ch[i].intervalMs) {
    ch[i].lastBlink = now;
    ch[i].ledState = !ch[i].ledState;
    digitalWrite(ledPins[i], ch[i].ledState);
  }
}

void loop() {
  for (int i = 0; i < 8; i++) {
    handleButton(i);
    handleBlink(i);
  }
}
//halla!