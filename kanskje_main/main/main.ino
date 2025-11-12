#include <Arduino.h>

#include "Queue.h"
#include "Request.h"
#include "Display.h"
#include "Pins.h"
#include "Encoder.h"
#include "stepper.h"

Display display(41, 40, 37, 36, 35, 34, 4);
Queue queue;
Encoder encoder;
int workingDirection = 0;
int stop;
Stepper stepper;


float elevatorPosition  = 0;
int firstStop = 0;
bool up = {false};
bool down = {false};
bool upAbove = {false};
bool downBelow = {false};
bool firstStopHit = {false};

void newUpRound()
{
  workingDirection = 1;
  firstStop = queue.firstUp();
  stop = firstStop;
  firstStopHit = {false};
}

void newDownRound()
{
  workingDirection = -1;
  firstStop = queue.firstDown();
  stop = firstStop;
  firstStopHit = {false};
}


void setup() 
{
  display.setup();
  encoder.setup();
  queue.attachDisplay(&display);

  for (int i = 0; i < 8; i++) 
  {
    pinMode(button[i], INPUT);
    pinMode(led[i], OUTPUT);
    digitalWrite(led[i], LOW);
  }
  

}



void loop() 
{
  int y = analogRead(yPin);
  if(y>700)
  {
    queue.UpRequestHall();
  }
  if(y<300)
  {
    queue.DownRequestHall();
  }

  encoder.ReadEncoder();
  float elevatorPosition  = encoder.getElevatorPosition();

  bool up = !queue.emptyUp();
  bool down = !queue.emptyDown();
  bool upAbove = !queue.emptyUpAbove(elevatorPosition);
  bool downBelow = !queue.emptyDownBelow(elevatorPosition);

  if (elevatorPosition - float(firstStop) > -0.02f && elevatorPosition - float(firstStop) < 0.02f)
  {
    firstStopHit = {true};
  }
  
  if (!up && !down)
  {
    workingDirection = 0;
  }

  if(firstStopHit)
  {
    if (workingDirection == 0)
    {
      if (up)
      {
        newUpRound();
      }
      else if (down)
      {
        newDownRound();
      }
    }
    else if (workingDirection == 1 && !upAbove)
    {
      if(down)
      {
        newDownRound();
      }
      else if (!down)
      {
        newUpRound();
      }
    }
    else if (workingDirection == -1 && !downBelow)
    {
      if(up)
      {
        newUpRound();
      }
      else if (!up)
      {
        newDownRound();
      }
    }
  }

  switch (workingDirection)
  {
    case -1:
        display.showFloor(stop, 15, 0);
        display.showDirection(Dir::Down, 15, 1);
        break;

    case 1:
        display.showFloor(stop, 15, 0);
        display.showDirection(Dir::Up, 15, 1);
        break;

    case 0:
        display.showNothing(15, 0);
        display.showNothing(15, 1);
        break;
  }


  
  
  //kjør til stop

  

  if (workingDirection == 1 && elevatorPosition - float(stop) > -0.02f && elevatorPosition - float(stop) < 0.02f)
  {
    queue.clearUp(stop);
    stop = queue.nextUp(encoder.getElevatorPosition());
  }
  else if (workingDirection == -1 && elevatorPosition - float(stop) > -0.02f && elevatorPosition - float(stop) < 0.02f)
  {
    queue.clearDown(stop);
    stop = queue.nextDown(encoder.getElevatorPosition());
  }


}

