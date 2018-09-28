#include "InputHandler.h"

InputHandler::InputHandler()
{
  pinMode(START, INPUT);
  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(LEFT, INPUT);
  pinMode(RIGHT, INPUT);
}

void InputHandler::init()
{
  dir = prevDir = xpos;
  timeOfUpRelease = timeOfDownRelease = 0;
  released = true;
}

void InputHandler::pollForStart()
{
  while (!digitalRead(START));
}

void InputHandler::update()
{
  if (digitalRead(UP))
  {
    // Detect double up press
    if (prevDir != zneg && (millis() - timeOfUpRelease < 150))
    {
      dir = zpos;
    }
    // Prevent direction switch if a double up press just occurred
    else if (prevDir != yneg && (dir != zpos || (millis() - timeOfUpRelease > 500)))
    {
      dir = ypos;
    }
    released = false;
  }
  else if (digitalRead(DOWN))
  {
    // Detect double down press
    if (prevDir != zpos && (millis() - timeOfDownRelease < 150))
    {
      dir = zneg;
    }
    // Prevent direction switch if a double down press just occurred
    else if (prevDir != ypos && (dir != zneg || (millis() - timeOfDownRelease > 500)))
    {
      dir = yneg;
    }
    released = false;
  }
  else if (digitalRead(RIGHT))
  {
    if (prevDir != xneg)
    {
      dir = xpos;
    }
    released = false;
  }
  else if (digitalRead(LEFT))
  {
    if (prevDir != xpos)
    {
      dir = xneg;
    }
    released = false;
  }
  // Detect up or down button releases
  else if (!released)
  {
    if (dir == ypos)
    {
      timeOfUpRelease = millis();
    }
    else if (dir == yneg)
    {
      timeOfDownRelease = millis();
    }
    released = true;
  }
}

void InputHandler::saveDir()
{
  prevDir = dir;
}
