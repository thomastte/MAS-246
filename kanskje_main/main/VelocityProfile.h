#ifndef VELOCITYPROFILE_H
#define VELOCITYPROFILE_H

class VelocityProfile {
public:
    // Constructor: set maximum velocity and acceleration (units per second)
    VelocityProfile(float maxVel, float maxAcc);

    // Set a new motion target, starting from currentPos
    void setTarget(float currentPos, float targetPos);

    // Call regularly with dt in seconds
    void update(float dt);

    // Accessors
    float getPosition() const;
    float getVelocity() const;
    float getTarget()   const;
    float getStart()    const;
    bool  isFinished()  const;

private:
    float vMax_;
    float aMax_;

    float pos_;     // current position
    float vel_;     // current velocity
    float target_;  // target position
    float start_;   // starting position
    bool  finished_;
};

#endif