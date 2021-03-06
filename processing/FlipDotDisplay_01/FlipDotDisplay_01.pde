import processing.serial.*;
import processing.video.*;
import java.util.*;

Serial myPort;
boolean serialConnected = false;

Movie testMovie;
PImage image;

int flipDotWidth = 28;
int flipDotHeight = 24;

int flipDotPixels[];
boolean displayImage = false;
boolean activatePixel = false;
int pixel = 0;

BitSet pixelValues;

void setup()
{
  size(560, 480);

  //initialize flipdot pixels
  flipDotPixels = new int[flipDotWidth*flipDotHeight];

  // print all available ports
  println(Serial.list());

  // open specific serial port
  try
  {
    myPort = new Serial(this, "/dev/cu.usbmodem14201", 250000);
    serialConnected = true;
    myPort.clear();
  }
  catch(Exception e)
  {
    println(e);
    serialConnected = false;
  }

  //load video
  testMovie = new Movie(this, "Flickermood.mp4");
  testMovie.loop();

  image = loadImage("image.png");
  
  pixelValues = new BitSet(flipDotWidth*flipDotHeight);
  
  frameRate(1);
}

void draw()
{
  image(testMovie, 0, 0);
  //background(0);

  if (displayImage)
  {
    image(image, 0, 0);
  }

  int scale = width/flipDotWidth;
  PVector position = new PVector();
  loadPixels();

  for (int x=0; x<flipDotWidth; x++)
  {
    for (int y=0; y<flipDotHeight; y++)
    {
      position.x = scale/2+x*scale;
      position.y = scale/2+y*scale;

      int loc = int(position.x) + int(position.y)*width;

      color currColor = pixels[loc];
      int fillColor;

      if (brightness(currColor)> 80)
      {
        fillColor = 255;
        pixelValues.set(loc);
      } else
      {
        fillColor = 0;
        pixelValues.clear(loc);
      }

      //fill(fillColor);
      //ellipse(scale/2+x*scale, scale/2+y*scale, scale, scale);

      int flipDotLoc = x+y*flipDotWidth;
      if (flipDotPixels[flipDotLoc] != fillColor)
      {
        sendValue(x, y, floor(fillColor/255));
      }
      flipDotPixels[flipDotLoc] = fillColor;
    }
  }
  println();
  println("-----------------------------------------------------------------");
}

void getPixelValue()
{
}

void sendValue(int _x, int _y, int _value)
{
  print("X:"+_x, " Y:"+_y+" Value:"+_value);

  if (serialConnected)
  {
    myPort.write(_x);
    myPort.write(_y);
    myPort.write(_value);
    delay(10);
    /*
    myPort.write(str(_x));
     myPort.write(",");
     myPort.write(str(_y));
     myPort.write(",");
     myPort.write(str(_value));
     myPort.write("\n");
     */
  }
}

void movieEvent(Movie m) {
  m.read();
}

void keyPressed()
{
  switch(key)
  {
  case 'a':
    displayImage =! displayImage;
    break;
  case 's':
    if (serialConnected)
    {
      activatePixel =! activatePixel;
      pixel++;
      //myPort.write(10);
      //myPort.write(10);
      //myPort.write(0);
      
      myPort.write(pixel);
      myPort.write(pixel);
      myPort.write(0);
    }
    break;
  }
}
