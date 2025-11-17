#include <Arduino.h>
#include <string.h>

#include "Queue.h"
#include "Request.h"
#include "Display.h"
#include "Pins.h"
#include "Encoder.h"
#include "stepper.h"

#include "DC.h"
#include "PID.h"

Display display(41, 40, 37, 36, 35, 34, 4);
Queue queue;
Encoder encoder;
DC dc;
PID pid;
int workingDirection = 0;
int stop;
Stepper stepper;
unsigned long door_timer;
bool open = false;


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
  stepper.setup(0.5, 0.5);
  encoder.setup();
  queue.attachDisplay(&display);
  Serial.begin(9600);

  for (int i = 0; i < 8; i++) 
  {
    pinMode(button[i], INPUT);
    pinMode(led[i], OUTPUT);
    digitalWrite(led[i], LOW);
  }
}



void loop() 
{
  dc.setup();
  pid.setup();

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
    stepper.doorOpen();
    stepper.update();
    door_timer = millis();
    open = true;
  }
  else if (workingDirection == -1 && elevatorPosition - float(stop) > -0.02f && elevatorPosition - float(stop) < 0.02f)
  {
    stepper.doorOpen();
    stepper.update();
    door_timer = millis();
    open = true;

  }

  if(open)
  {
    if(workingDirection == 1)
    {
      int intfrom = round(elevatorPosition);
      for (int i = intfrom+1; i < 8; ++i)
      {
        digitalWrite(led[i], HIGH);
        if (digitalRead(button[i]) == HIGH)
        {
          queue.addRequest(Request(i, Dir::Up));
        }
        
      }
    }
    
    if (workingDirection == -1)
    {
      int intfrom = round(elevatorPosition);
      for (int i = intfrom-1; i >= 0; --i)
      {
        digitalWrite(led[i], HIGH);
        if (digitalRead(button[i]) == HIGH)
        {
          queue.addRequest(Request(i, Dir::Down));
        }
      }
    }
  }

  if (millis() - door_timer >= 5000 && open)
  {
    Serial.println(millis() - door_timer);
    open = false;
    stepper.doorClose();
    stepper.update();
    if (workingDirection == 1)
    {
      queue.clearUp(stop);
      stop = queue.nextUp(elevatorPosition);
    }
    if (workingDirection == 1)
    {
      queue.clearDown(stop);
      stop = queue.nextDown(elevatorPosition);
    }
  }
  

/*delay(5000);
stepper.update();
stepper.doorStop();
delay(5000);
stepper.update();
stepper.doorClose();*/
}

