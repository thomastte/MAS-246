#include "VelocityProfile.h"
#include <math.h>
float pos;

VelocityProfile::VelocityProfile(float maxVel, float maxAcc)
    : vMax_(maxVel),
      aMax_(maxAcc),
      pos_(0.0f),
      vel_(0.0f),
      target_(0.0f),
      start_(0.0f),
      finished_(true)
{
}

void VelocityProfile::setTarget(float currentPos, float targetPos)
{
    start_   = currentPos;
    pos_     = currentPos;
    target_  = targetPos;
    vel_     = 0.0f;
    finished_ = false;
}

void VelocityProfile::update(float dt)
{
    if (finished_ || dt <= 0.0f) {
        vel_ = 0.0f;
        return;
    }

    float distance = target_ - pos_;
    float dir      = (distance >= 0.0f) ? 1.0f : -1.0f;

    float vMag = fabsf(vel_);

    // distance needed to stop with max speed and max acceleration
    float dStop = (vMag * vMag) / (2.0f * aMax_);

    if (fabsf(distance) <= dStop) {
        // Need to decelerate
        vMag -= aMax_ * dt;
        if (vMag < 0.0f) vMag = 0.0f;
    } else {
        // Can accelerate up to max speed
        vMag += aMax_ * dt;
        if (vMag > vMax_) vMag = vMax_;
    }

    vel_ = dir * vMag;
    pos_ += vel_ * dt;

    bool reachedTarget =
        (dir > 0.0f && pos_ >= target_) ||
        (dir < 0.0f && pos_ <= target_);

    if (reachedTarget) {
        pos_      = target_;
        vel_      = 0.0f;
        finished_ = true;
    }
}

float VelocityProfile::getPosition() const
{
    return pos_;
}

float VelocityProfile::getVelocity() const
{
    return vel_;
}

float VelocityProfile::getTarget() const
{
    return target_;
}

float VelocityProfile::getStart() const
{
    return start_;
}

bool VelocityProfile::isFinished() const
{
    return finished_;
}
