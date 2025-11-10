#include "Queue.h"
#include "Display.h"
#include <Arduino.h>

//___________Knapper,_lys_og_joystick_før_settup_______________
const int led[8] = {49, 48, 47, 46, 45, 44, 43, 42};
const int button[8] = {22, 23, 24, 25, 26, 27, 28, 29};
const int yPin = A2; // Y direction joystick


void Queue::UpRequestHall ()
{
  for (int i = 0; i < 8; i++) 
    {
      if (digitalRead(button[i]) == HIGH)
      {
        if (i == 7)
        {
          // nothing
        }
        else
        {
          addRequest(Request(i, Dir::Up));
          if (display_) 
          {
            display_->showFloor(i,0,0);
            display_->showDirection(Dir::Up,1,0);
          }
        }
      }
    }
}

void Queue::DownRequestHall ()
{
  for (int i = 0; i < 8; i++) 
    {
      if (digitalRead(button[i]) == HIGH)
      {
        if (i == 0)
        {
          // nothing
        }
        else
        {
          addRequest(Request(i, Dir::Down));
          if (display_) 
          {
            display_->showFloor(i,0,0);
            display_->showDirection(Dir::Down,1,0);
          }
        }
      }
    }
}


//_______________________________________________
void Queue::addRequest(const Request& request) {
    int floor = request.floor();
    if (floor < 0 || floor >= NUM_FLOORS) return;

    if (request.direction() == Dir::Up)
        up_[floor] = true;
    else
        down_[floor] = true;
}



//_______________________________________________
int Queue::nextUp(float from) const 
{
  // må kanskje legge inn litt justering på hva den gotar for mindre nøyaktighet 
    for (int i = from ; i < NUM_FLOORS; ++i) 
    {
      if (up_[i])
      {
        if (display_) display_->showFloor(i,15,0);
        return i;
      }
    }
    return -1;
}

int Queue::nextDown(float from) const 
{
  // må kanskje legge inn litt justering på hva den gotar for mindre nøyaktighet 
    for (int i = from ; i >= 0; --i) 
    {
      if (down_[i])
      {
        if (display_) display_->showFloor(i,15,0);
        return i;
      }
    }
    return -1;
}









//_______________________________________________
void Queue::clearUp(int floor) 
{
  up_[floor] = false;
}
void Queue::clearDown(int floor) 
{
  down_[floor] = false;
}


//_______________________________________________
bool Queue::emptyUp() const 
{
    for (int i = 0; i < NUM_FLOORS; ++i) 
    {
        if (up_[i]) 
        {
          return false;
        }
    }
    return true;
}

bool Queue::emptyDown() const 
{
    for (int i = 0; i < NUM_FLOORS; ++i) 
    {
        if (down_[i]) 
        {
          return false;
        }
    }
    return true;
}

int Queue::workingDirection()
{
  for (int i = 0; i < NUM_FLOORS; ++i)
  {
    if (up_[i])
    {
      if (display_) display_->showDirection(Dir::Up, 15, 1);
      return 1;
    }
  }
    
  for (int i = 0; i < NUM_FLOORS; ++i)
  {
    if (down_[i])
    {
      if (display_) display_->showDirection(Dir::Down, 15, 1);
      return -1;
    }
  }
   return 0; 
}


int Queue::firstUp() const 
{
  for (int i = 0; i < NUM_FLOORS; ++i)
  {
    if (up_[i]) return i;
  } 
}

int Queue::firstDown() const 
{
  for (int i = NUM_FLOORS - 1; i >= 0; --i)
  {
    if (down_[i]) return i;
  } 
}

