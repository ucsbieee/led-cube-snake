#include "Snake.h"

Snake::Snake()
{
  // Start from a known state
  reset();
}

void Snake::start()
{
  io.pollForStart();
  init();
}

void Snake::update()
{
  io.saveDir();
  // Repeat the display and input polling for the duration of one frame
  unsigned long startTime = millis();
  unsigned long elapsed = 0;
  while (elapsed < frameTime)
  {
    elapsed = millis() - startTime;
    cube.display();
    io.update();
  }
  // Reset the buffer and prepare data for the next frame
  cube.clearBuffer();
  logic();
}

void Snake::reset()
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

void Snake::init()
{
  // Init runtime variables
  io.init();
  dead = false;
  frameTime = 500;
  
  // Init snake location
  snake.push_front({0, 0, 0});
  m[snake.front().x][snake.front().y][snake.front().z] = 1;
  
  // Spawn a food
  generateFood();
  
  // Illuminate the snake and food LEDs
  bufferFromMatrix();
}

void Snake::logic()
{
  Coord next;

  // Check if the next location is valid
  switch (io.getDir())
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
  m[next.x][next.y][next.z] = 1;
  snake.push_front(next);

  // Write the data
  bufferFromMatrix();
}

void Snake::generateFood()
{
  do
  {
    food = { random(4), random(4), random(4) };
  } while (m[food.x][food.y][food.z]);
  
  m[food.x][food.y][food.z] = 1;
}

void Snake::bufferFromMatrix()
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
