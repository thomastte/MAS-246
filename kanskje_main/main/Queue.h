#ifndef QUEUE_H
#define QUEUE_H

#include "Request.h"

class Display;//heter displayet

class Queue {
public:
    static const int NUM_FLOORS = 8;

    void attachDisplay(Display* d) { display_ = d; }
    void UpRequestHall();
    void DownRequestHall();
    void addRequest(const Request& request);
    int  nextUp(float from) const;
    int  nextDown(float from) const;
    void clearUp(int floor);
    void clearDown(int floor);
    bool emptyUp() const;
    bool emptyDown() const;
    int workingDirection();
    int firstUp() const;
    int firstDown() const;
private:
    bool up_[NUM_FLOORS]   = {false};
    bool down_[NUM_FLOORS] = {false};
    Display* display_ = nullptr;
};

#endif
