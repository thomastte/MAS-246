#include <LiquidCrystal.h>

//(RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(41, 40, 37, 36, 35, 34);

const int LCD_BACKLIGHT = 4;

const int yPin = A2; // Y direction joystick

const int led[8] = {49, 48, 47, 46, 45, 44, 43, 42};
const int button[8] = {22, 23, 24, 25, 26, 27, 28, 29};


class Request;                
extern Request queue[5];   
extern int queueCount;






class Request
{
  public:

  int target;
  int direction; 

  void OutsideUpRequest()
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
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Up floor ");
          lcd.print(i);
          
          target = i;     
          direction  = 1;  

          queue[queueCount].target = target;
          queue[queueCount].direction = direction;
          queueCount++;
          if (queueCount > 4)
          {
            queueCount = 4;
          }   

        }
      }
    }
  }
  void OutsideDownRequest()
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
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Down floor ");
          lcd.print(i);

          target = i;      
          direction = -1;

          queue[queueCount].target = target;
          queue[queueCount].direction = direction;
          queueCount++;
          if (queueCount > 4)
          {
            queueCount = 4;
          } 

        }
      }
    }
  }

};






Request request;
Request queue[5];  
int queueCount = 0;

void setup() 
{
  for (int i = 0; i < 8; i++) 
  {
    pinMode(button[i], INPUT);
    pinMode(led[i], OUTPUT);
    digitalWrite(led[i], LOW);
  }

  pinMode(LCD_BACKLIGHT, OUTPUT);
  digitalWrite(LCD_BACKLIGHT, HIGH);
  
  lcd.begin(16, 2);  // 16x2 display
  lcd.clear();
}

void loop() 
{
  int y = analogRead(yPin);

  // Requests
  if (y < 300)
  {
    request.OutsideDownRequest();
  }

  if (y > 700)
  {
    request.OutsideUpRequest();
  }



  
}
