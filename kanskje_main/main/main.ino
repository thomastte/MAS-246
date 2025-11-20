#include <Arduino.h>
#include <string.h>

#include "Queue.h"
#include "Request.h"
#include "Display.h"
#include "Pins.h"
#include "Encoder.h"
#include "stepper.h"
#include "VelocityProfile.h"
#include "DC.h"
#include "PID.h"

Display display(41, 40, 37, 36, 35, 34, 4);
Queue queue;
Encoder encoder;
DC dc;
PID pid;

unsigned long lastProfileMs = 0;
int workingDirection = 0;
int stop;
Stepper stepper;
unsigned long door_timer;
bool open = false;
float profilePos;
float profileVel;

float elevatorPosition  = 0;
int firstStop = 0;
bool up = {false};
bool down = {false};
bool upAbove = {false};
bool downBelow = {false};
bool firstStopHit = {false};

// --- Trapezoidal motion profile (ADDED) ---
const float MAX_VEL = 0.2f;   // changes velocity profile
const float MAX_ACC = 0.05f;  // changes velocity profile

VelocityProfile motion(MAX_VEL, MAX_ACC);  // profile object

float tMotion = 0.0f;
// --- end added ---

void sikteSignal();
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
  dc.setup();
  pid.setup();
  queue.attachDisplay(&display);
  Serial.begin(9600);

  lastProfileMs = millis(); // for velocity profile timing

  for (int i = 0; i < 8; i++) 
  {
    pinMode(button[i], INPUT);
    pinMode(led[i], OUTPUT);
    digitalWrite(led[i], LOW);
  }
}



void loop() 
{
  sikteSignal();

  int y = analogRead(yPin);
  if (y > 700)
  {
    queue.UpRequestHall();
  }
  if (y < 300)
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

  if (firstStopHit)
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
      if (down)
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
      if (up)
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

  // kjør til stop
  if(pid.loop(motion.getPosition(),elevatorPosition)>0)
  {
    dc.moveElevator(1, pid.loop(motion.getPosition(),elevatorPosition));
  }
  else if(pid.loop(motion.getPosition(),elevatorPosition)<0)
  {
    dc.moveElevator(-1, 0-pid.loop(motion.getPosition(),elevatorPosition));
  }
  


  if (!open && workingDirection == 1 && elevatorPosition - float(stop) > -0.02f && elevatorPosition - float(stop) < 0.02f)
  {
    stepper.doorOpen();
    stepper.update();
    door_timer = millis();
    open = true;
  }
  else if (!open && workingDirection == -1 && elevatorPosition - float(stop) > -0.02f && elevatorPosition - float(stop) < 0.02f)
  {
    stepper.doorOpen();
    stepper.update();
    door_timer = millis();
    open = true;
  }

  if (open)
  {
    if (workingDirection == 1)
    {
      int intfrom = round(elevatorPosition);
      for (int i = intfrom + 1; i < 8; ++i)
      {
        digitalWrite(led[i], HIGH);
        if (digitalRead(button[i]) == HIGH)
        {
          queue.addRequest(Request(i, Dir::Up));
          display.showFloor(i, 0, 0);
          display.showDirection(Dir::Up, 1, 0);
          queue.nextUp(elevatorPosition);
        }
      }
    }
    
    if (workingDirection == -1)
    {
      int intfrom = round(elevatorPosition);
      for (int i = intfrom - 1; i >= 0; --i)
      {
        digitalWrite(led[i], HIGH);
        if (digitalRead(button[i]) == HIGH)
        {
          queue.addRequest(Request(i, Dir::Down));
          display.showFloor(i, 0, 0);
          display.showDirection(Dir::Down, 1, 0);
          queue.nextDown(elevatorPosition);
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
    for (int i = 0; i < 8; ++i)
    {
      digitalWrite(led[i], LOW);
    }
    if (workingDirection == 1)
    {
      queue.clearUp(stop);
      stop = queue.nextUp(elevatorPosition);
    }
    if (workingDirection == -1)
    {
      queue.clearDown(stop);
      stop = queue.nextDown(elevatorPosition);
    }
  }

  /*
  delay(5000);
  stepper.update();
  stepper.doorStop();
  delay(5000);
  stepper.update();
  stepper.doorClose();
  */
}

void sikteSignal()
{
   // --- Trapezoidal motion towards current stop (ADDED) ---
  static int lastProfileStop = -1;

  unsigned long motionNow = millis();
  float motionDt = (motionNow - lastProfileMs) / 1000.0f;   // seconds
  lastProfileMs = motionNow;

  if (workingDirection == 0)
  {
    // No movement command, keep profile idle
  }
  else
  {
    // Only start a new profile if the target floor changed
    if (lastProfileStop != stop) {
      motion.setTarget(encoder.getElevatorPosition(), (float)stop);
      lastProfileStop = stop;
      tMotion = 0.0f;
    }

    // Only run / print while the profile is still active
    if (motionDt > 0.0f && !motion.isFinished())
    {
      motion.update(motionDt);
      tMotion += motionDt;

      profilePos = motion.getPosition();
      profileVel = motion.getVelocity();

      // Debug output: time, pos, vel
      
      Serial.print(tMotion);
      Serial.print(", ");
      Serial.print(profilePos);
      Serial.print(", ");
      Serial.print(profileVel);
      Serial.print(", ");
      Serial.print(stop);
      Serial.print(", ");

      // TODO: use profileVel with DC + PID here if you want:
      // dc.setVelocitySetpoint(profileVel);
      // pid.update(...);
    }
  }
  // --- end added ---

}
