/*
    Copyright (C) 2021 kempermo


    arduinoNanoReceiver is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "wwFlipGFX.h"
#include "fonts/dotCousine_Bold_v02.h"
wwFlipGFX ww;

#define DELAYPOWERON 5000

String textToDisplay = "";
boolean newTextAvailable = false;

void setup()
{
  Serial.begin(9600);

  ww.begin();
  ww.dotPowerOn();
  delay(DELAYPOWERON);
  ww.setCoilResetDuration(800);
  ww.setCoilSetDuration(800);
  ww.setCoilFlipDuration(700);
  ww.setAll(0);
  ww.resetAll(0);

  ww.mSetFont(dotCousine_Bold_v02);
}

void loop()
{
  while (Serial.available())
  {
    textToDisplay = Serial.readString();
    newTextAvailable = true;
  }

  if (newTextAvailable)
  {
    ww.setAll(0);
    ww.resetAll(0);

    ww.mReset();
    ww.mDrawString(10, 1, textToDisplay);
    ww.mUpdate();

    newTextAvailable = false;
  }
}
