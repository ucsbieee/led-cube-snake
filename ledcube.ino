/*
 * File:     ledcube.ino
 * Author:   Jeff Longo
 * Comments: UCSB IEEE LED cube snake project
*/

#include "Cube.h"

#define START 8
#define UP    9
#define DOWN  10
#define LEFT  11
#define RIGHT 12

Cube cube;

void setup() 
{
  Serial.begin(9600);
  
  pinMode(START, INPUT);
  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(LEFT, INPUT);
  pinMode(RIGHT, INPUT);

  // Start the cube from a known state 
  cube.reset();
}

void loop() 
{
  for (int k = 0; k < 4; k++)
  {
    cube.bufferLED(0, 0, k);
  }
  cube.display();
}
