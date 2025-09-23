// ElevatorQueue.ino
// Queue and dispatcher for an elevator
// Uses bitsets for requests and the SCAN style rule set

#include <Arduino.h>

const int floorCount = 8;        // change to match your building, max 16 with uint16_t
const int invalidFloor = -1;

enum Direction { DirDown = -1, DirIdle = 0, DirUp = 1 };

class ElevatorQueue {
public:
  ElevatorQueue()
    : car(0), hallUp(0), hallDown(0),
      current(0), dir(DirIdle) {}

  // Add requests
  void requestCar(int f)       { setBit(car, f); }
  void requestHallUp(int f)    { setBit(hallUp, f); }
  void requestHallDown(int f)  { setBit(hallDown, f); }

  // Clear requests at a floor once served
  void clearAt(int f) {
    clearBit(car, f);
    clearBit(hallUp, f);
    clearBit(hallDown, f);
  }

  // Update current floor and optionally adjust direction
  void setCurrentFloor(int f)  { if (inRange(f)) current = f; }

  // Decide where to go next based on current direction and outstanding calls
  // Returns a floor index or invalidFloor if nothing to do
  int nextTarget() {
    if (!hasAnyRequest()) {
      dir = DirIdle;
      return invalidFloor;
    }

    // Serve current floor if there is any request that matches direction or car call
    if (hasStopHere(current)) {
      return current;
    }

    // If idle, choose a direction toward the nearest request
    if (dir == DirIdle) chooseInitialDirection();

    // Try to continue in the same direction
    int next = (dir == DirUp) ? findAbove(current + 1) : findBelow(current - 1);
    if (next != invalidFloor) return next;

    // Nothing ahead in this direction, reverse
    dir = (dir == DirUp) ? DirDown : DirUp;
    next = (dir == DirUp) ? findAbove(current + 1) : findBelow(current - 1);
    if (next != invalidFloor) return next;

    // Fallback, should only happen if current floor is the only pending stop
    if (hasStopHere(current)) return current;

    // No requests remain
    dir = DirIdle;
    return invalidFloor;
  }

  // Getters
  Direction direction() const { return dir; }
  bool hasAnyRequest() const  { return (car | hallUp | hallDown) != 0; }

private:
  uint16_t car;       // car calls
  uint16_t hallUp;    // hall up calls
  uint16_t hallDown;  // hall down calls

  int current;        // current floor index
  Direction dir;      // current travel dir
