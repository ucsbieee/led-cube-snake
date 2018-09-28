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
}
