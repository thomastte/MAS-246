#include <Arduino.h>
#include <math.h>

// --- Config ---
const float MAX_VEL = 10.0f;
const float MAX_ACC = 5.0f;
float targetPos     = -100.0f;

// --- Class definition (can also be moved to a .h file) ---
class TrapezoidalProfile {
public:
    TrapezoidalProfile(float maxVel, float maxAcc)
        : vMax(maxVel), aMax(maxAcc),
          pos(0.0f), vel(0.0f), target(0.0f),
          startPos(0.0f), finished(true) {}

    void setTarget(float currentPos, float targetPos) {
        startPos = currentPos;
        pos      = currentPos;
        target   = targetPos;
        vel      = 0.0f;
        finished = false;
    }

    void update(float dt) {
        if (finished || dt <= 0.0f) {
            vel = 0.0f;
            return;
        }

        float distance = target - pos;
        float dir      = (distance >= 0.0f) ? 1.0f : -1.0f;

        float vMag = fabs(vel);
        float dStop = (vMag * vMag) / (2.0f * aMax);

        if (fabs(distance) <= dStop) {
            vMag -= aMax * dt;          // decelerate
            if (vMag < 0.0f) vMag = 0.0f;
        } else {
            vMag += aMax * dt;          // accelerate
            if (vMag > vMax) vMag = vMax;
        }

        vel = dir * vMag;
        pos += vel * dt;

        bool reachedTarget =
            (dir > 0.0f && pos >= target) ||
            (dir < 0.0f && pos <= target);

        if (reachedTarget) {
            pos = target;
            vel = 0.0f;
            finished = true;
        }
    }

    float getPosition() const { return pos; }
    float getVelocity() const { return vel; }
    float getTarget()   const { return target; }
    float getStart()    const { return startPos; }
    bool  isFinished()  const { return finished; }

private:
    float vMax;
    float aMax;

    float pos;
    float vel;
    float target;
    float startPos;
    bool  finished;
};

// --- Globals ---
TrapezoidalProfile motion(MAX_VEL, MAX_ACC);

unsigned long lastTimeMs    = 0;
float tMotion               = 0.0f; 
bool  printedStopSample     = false;
float startPosGlobal        = 0.0f;

// --- setup / loop ---
void setup() {
    Serial.begin(9600);
    Serial.println("t,pos,vel");

    // If you had an encoder, you'd read it here instead of 0.0f
    startPosGlobal = 0.0f;

    motion.setTarget(startPosGlobal, targetPos);

    unsigned long now = millis();
    lastTimeMs        = now;
    tMotion           = 0.0f;
    printedStopSample = false;
}

void loop() {
    unsigned long now = millis();
    float dt = (now - lastTimeMs) / 1000.0f;
    lastTimeMs = now;

    motion.update(dt);

    float pos     = motion.getPosition();
    float vel     = motion.getVelocity();
    bool finished = motion.isFinished();

    if (!finished) {
        tMotion += dt;
    }

    if (!finished || !printedStopSample) {
        Serial.print(tMotion);
        Serial.print(",");
        Serial.print(pos);
        Serial.print(",");
        Serial.println(vel);

        if (finished) {
            printedStopSample = true;
        }
    }
}
