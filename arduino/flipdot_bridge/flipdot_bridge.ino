#include <Wire.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "flipdot.h"

#define DISPLAY_WIDTH   56
#define DISPLAY_HEIGHT  24
#define AMOUNT_OF_PANELS 2

#define NUM_VALUES 3
int incomingValues[NUM_VALUES];

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(250000);
  Serial.println("START");
  flipdot_setup(28, 24, AMOUNT_OF_PANELS);
  flipdot_cls(0);
  pinMode(13, OUTPUT);
}

void loop(void)
{

  // check if data is available
  if (Serial.available() == 3)
  {
    incomingValues[0] = Serial.read();
    incomingValues[1] = Serial.read();
    incomingValues[2] = Serial.read();

    if (incomingValues[0] < DISPLAY_WIDTH && incomingValues[1] <= DISPLAY_HEIGHT)
    {
      flipdot_pixel(incomingValues[0], incomingValues[1], incomingValues[2]);
    }
  }

  /*
    for (int x = 0; x < DISPLAY_WIDTH; x++)
    {
    for (int y = 0; y <= DISPLAY_HEIGHT; y++)
    {
      flipdot_pixel(x, y, 1);
      delay(5);
      if (y == 0)
      {
        flipdot_pixel(x - 1, DISPLAY_HEIGHT, 0);
      }
      else
      {
        flipdot_pixel(x, y - 1, 0);
      }
      delay(0);
      //Serial.println(y);
    }
    }

    for (int y = 0; y <= DISPLAY_HEIGHT; y++)
    {
    for (int x = 0; x < DISPLAY_WIDTH; x++)
    {
      flipdot_pixel(x, y, 1);

        delay(5);
        if (y == 0)
        {
        flipdot_pixel(x - 1, DISPLAY_HEIGHT, 0);
        }
        else
        {
        flipdot_pixel(x, y - 1, 0);
        }
        delay(0);
        //Serial.println(y);
    }
    }

    flipdot_cls(0);
    delay(1000);
    flipdot_cls(1);
    delay(1000);
  */
}

void split(String inputString, int returnData[], int numOfValues)
{
  int index = 0;
  int lastPos = 0;

  for (int i = 0; i < inputString.length(); i++)
  {
    if (inputString.charAt(i) == ',' && index < numOfValues)
    {
      String tempStr = inputString.substring(lastPos, i - 1);
      returnData[index] = tempStr.toInt();
      index++;
      lastPos = i + 1;
    }
  }
}

