#ifndef VELOCITYPROFILE_H
#define VELOCITYPROFILE_H

class VelocityProfile {
public:
    //set maximum velocity and acceleration (units per second)
    VelocityProfile(float maxVel, float maxAcc);

    // Set a new motion target, starting from current position
    void setTarget(float currentPos, float targetPos);

    void update(float dt);

    float getPosition() const;
    float getVelocity() const;
    float getTarget()   const;
    float getStart()    const;
    bool  isFinished()  const;

private:
    float vMax_;
    float aMax_;

    float pos_;     // position
    float vel_;     // velocity
    float target_;  // target position
    float start_;   // starting position
    bool  finished_;
};

#endif