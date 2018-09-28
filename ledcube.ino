/*
   File:     ledcube.ino
   Author:   Jeff Longo
   Comments: UCSB IEEE LED cube snake project
*/

#include <ArduinoSTL.h>
#include <deque>
#include "Cube.h"

// Begin pin definitions
#define START 8
#define UP    9
#define DOWN  10
#define LEFT  11
#define RIGHT 12
// End pin definitions

// Begin custom data types
struct Coord {
  byte x, y, z;
};
enum Direction { xpos, xneg, ypos, yneg, zpos, zneg };
// End custom data types

// Begin global variables
Cube cube;
bool m[4][4][4];
bool dead;
std::deque<Coord> snake;
Coord food;
Direction dir;
bool released;
unsigned long timeOfUpRelease, timeOfDownRelease;
int frameTime;
// End global variables

void setup()
{
  Serial.begin(9600);

  pinMode(START, INPUT);
  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(LEFT, INPUT);
  pinMode(RIGHT, INPUT);

  reset();
}

void loop()
{
  pollForStart();
  initialize();
  while (!dead)
  {
    update();
  }
  reset();
}

void pollForStart()
{
  while (!digitalRead(START));
}

void initialize()
{
  dead = false;
  frameTime = 500;
  released = true;
  dir = xpos;
  // Init snake location
  snake.push_front({0, 0, 0});
  m[snake.front().x][snake.front().y][snake.front().z] = 1;
  // Spawn a food
  generateFood();
  // Illuminate the snake and food LEDs
  bufferFromMatrix();
}

void update()
{
  // Repeat the display and input polling for the duration of one frame
  unsigned long startTime = millis();
  unsigned long elapsed = 0;
  while (elapsed < frameTime)
  {
    elapsed = millis() - startTime;
    cube.display();
    handleInput();
  }
  // Reset the buffer and prepare data for the next frame
  cube.clearBuffer();
  gameLogic();
}

void handleInput()
{
  if (digitalRead(UP))
  {
    // Detect double up press
    if (dir != zneg && (millis() - timeOfUpRelease < 100))
    {
      dir = zpos;
    }
    // Prevent direction switch if a double up press just occurred
    else if (dir != yneg && (dir != zpos || (millis() - timeOfUpRelease > 500)))
    {
      dir = ypos;
    }
    released = false;
  }
  else if (digitalRead(DOWN))
  {
    // Detect double down press
    if (dir != zpos && (millis() - timeOfDownRelease < 100))
    {
      dir = zneg;
    }
    // Prevent direction switch if a double down press just occurred
    else if (dir != ypos && (dir != zneg || (millis() - timeOfDownRelease > 500)))
    {
      dir = yneg;
    }
    released = false;
  }
  else if (digitalRead(RIGHT))
  {
    if (dir != xneg)
    {
      dir = xpos;
    }
    released = false;
  }
  else if (digitalRead(LEFT))
  {
    if (dir != xpos)
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

void gameLogic()
{
  Coord next;
  switch (dir)
  {
    case xpos:
      {
        next = { snake.front().x + 1, snake.front().y, snake.front().z };
        if (next.x > 3)
        {
          dead = true;
          return;
        }
      }
      break;
    case xneg:
      {
        next = { snake.front().x - 1, snake.front().y, snake.front().z };
        if (next.x < 0)
        {
          dead = true;
          return;
        }
      }
      break;
    case ypos:
      {
        next = { snake.front().x, snake.front().y + 1, snake.front().z };
        if (next.y > 3)
        {
          dead = true;
          return;
        }
      }
      break;
    case yneg:
      {
        next = { snake.front().x, snake.front().y - 1, snake.front().z };
        if (next.y < 0)
        {
          dead = true;
          return;
        }
      }
      break;
    case zpos:
      {
        next = { snake.front().x, snake.front().y, snake.front().z + 1 };
        if (next.z > 3)
        {
          dead = true;
          return;
        }
      }
      break;
    case zneg:
      {
        next = { snake.front().x, snake.front().y, snake.front().z - 1 };
        if (next.z < 0)
        {
          dead = true;
          return;
        }
      }
      break;
    default:
      dead = true;
      return;
  }

  // If the food is captured, do not delete the tail
  if (next.x == food.x && next.y == food.y && next.z == food.z)
  {
    generateFood();
  }
  // The snake ran into the body
  else if (m[next.x][next.y][next.z])
  {
    dead = true;
    return;
  }
  else
  {
    m[snake.back().x][snake.back().y][snake.back().z] = 0;
    snake.pop_back();
  }
  
  // Set the next position as the new head
  snake.push_front(next);
  m[next.x][next.y][next.z] = 1;

  // Write the data
  bufferFromMatrix();
}

void generateFood()
{
  do
  {
    food = { random(4), random(4), random(4) };
  } while (m[food.x][food.y][food.z]);
  m[food.x][food.y][food.z] = 1;
}

void bufferFromMatrix()
{
  for (byte x = 0; x < 4; x++)
  {
    for (byte y = 0; y < 4; y++)
    {
      for (byte z = 0; z < 4; z++)
      {
        if (m[x][y][z])
        {
          cube.bufferLED(x, y, z);
        }
      }
    }
  }
}

void reset()
{
  snake.clear();
  cube.reset();

  for (byte x = 0; x < 4; x++)
  {
    for (byte y = 0; y < 4; y++)
    {
      for (byte z = 0; z < 4; z++)
      {
        m[x][y][z] = 0;
      }
    }
  }
}
