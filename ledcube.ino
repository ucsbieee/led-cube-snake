/*
   File:     ledcube.ino
   Author:   Jeff Longo
   Comments: UCSB IEEE LED cube snake project
*/

#include "Snake.h"

Snake snake;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  snake.start();
  while (!snake.isDead())
  {
    snake.update();
  }
  snake.reset();
  /*
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      for (int k = 0; k < 4; k++)
      {
        cube.bufferLED(k, j, i);
        cube.display();
      }
  cube.reset();
  */
        
}
