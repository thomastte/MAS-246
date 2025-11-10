#include <Arduino.h>

#include "Queue.h"
#include "Request.h"
#include "Display.h"
#include "Pins.h"
#include "Encoder.h"

Display display(41, 40, 37, 36, 35, 34, 4);
Queue queue;
Encoder encoder;
int workingDirection = 0;
int stop = 0;




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




}

