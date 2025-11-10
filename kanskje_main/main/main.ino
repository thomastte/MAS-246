#include <Arduino.h>
#include <string.h>

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
unsigned long door_timer;
bool open = false;


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

  if (queue.emptyUp() && queue.emptyDown())
  {
    workingDirection = 0;
  }
  else if (workingDirection != 1 && !queue.emptyUp() && queue.emptyDown())
  {
    workingDirection = 1;
  }
  else if (workingDirection != -1 && queue.emptyUp() && !queue.emptyDown())
  {
    workingDirection = -1;
  }
  else if (workingDirection == 1 && queue.emptyUpAbove(encoder.getElevatorPosition()) && !queue.emptyDown())
  {
    workingDirection = -1;
  }
  else if (workingDirection == -1 && !queue.emptyUp() && queue.emptyDownBelow(encoder.getElevatorPosition()))
  {
    workingDirection = 1;
  }
  else if (workingDirection == -1 && queue.emptyUp() && !queue.emptyDown() && queue.emptyDownBelow(encoder.getElevatorPosition()))
  {
    workingDirection = -1;
  }
  else if (workingDirection == 1 && !queue.emptyUp() && queue.emptyDown() && queue.emptyUpAbove(encoder.getElevatorPosition()))
  {
    workingDirection = -1;
  }

  switch (workingDirection)
  {
    case -1:
        stop = queue.firstDown();
        display.showFloor(stop, 15, 0);
        display.showDirection(Dir::Down, 15, 1);
        break;

    case 1:
        stop = queue.firstUp();
        display.showFloor(stop, 15, 0);
        display.showDirection(Dir::Up, 15, 1);
        break;

    case 0:
        display.showNothing(15, 0);
        display.showNothing(15, 1);
        break;
  }


  //kjør til stop

  if (workingDirection == 1 && encoder.getElevatorPosition() - float(stop) > -0.02f && encoder.getElevatorPosition() - float(stop) < 0.02f)
  {
    queue.clearUp(stop);
    stop = queue.nextUp(encoder.getElevatorPosition());
    display.showDirection(Dir::Up, 15, 1);
    display.showFloor(stop, 15, 0);
    stepper.doorOpen();
    stepper.update();
    door_timer = millis();
    open = true;
  }
  else if (workingDirection == -1 && encoder.getElevatorPosition() - float(stop) > -0.02f && encoder.getElevatorPosition() - float(stop) < 0.02f)
  {
    queue.clearDown(stop);
    stop = queue.nextDown(encoder.getElevatorPosition());
    display.showDirection(Dir::Down, 15, 1);
    display.showFloor(stop, 15, 0);
    stepper.doorOpen();
    stepper.update();
    door_timer = millis();
    open = true;
  }

  if (millis() - door_timer >= 5000 && open)
  {
    Serial.println(millis() - door_timer);
    open = false;
    stepper.doorClose();
    stepper.update();
  }
  

/*delay(5000);
stepper.update();
stepper.doorStop();
delay(5000);
stepper.update();
stepper.doorClose();*/
}

