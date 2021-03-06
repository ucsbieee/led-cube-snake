#include "Snake.h"

Snake::Snake()
{
  // Start from a known state
  reset();
}

void Snake::start()
{
  int i = 0, k = 1;
  while (!io.startPressed())
  {
    // Play a start pattern
    cube.bufferLED(i, 0, 0);
    cube.bufferLED(3 - i, 0, 1);
    cube.bufferLED(i, 0, 2);
    cube.bufferLED(3 - i, 0, 3);
    i += k*1;
    if (i >= 3 || i <= 0) k *= -1;

    // Update the display
    unsigned long startTime = millis();
    unsigned long elapsed = 0;
    while (elapsed < 100)
    {
      cube.display();
      io.update();
      elapsed = millis() - startTime;
    }
    cube.clearBuffer();
  }
  init();
}

void Snake::update()
{
  io.saveDir();
  // Repeat the display and input polling for the duration of one frame
  unsigned long startTime = millis();
  unsigned long elapsed = 0;
  int blinkCounter = 0;
  while (elapsed < frameTime)
  {
    // Blink the food LED every 10 updates
    if (blinkCounter++ % 10 == 0)
    {
      cube.toggleLED(food.x, food.y, food.z);
    }
    cube.display();
    io.update();
    elapsed = millis() - startTime;
  }
  // Reset the buffer and prepare data for the next frame
  cube.clearBuffer();
  logic();

  // If the snake dies, pause the display before ending the game
  if (dead)
  {
    cube.bufferFromMatrix(m);
    cube.bufferLED(food.x, food.y, food.z);
    startTime = millis();
    elapsed = 0;
    while (elapsed < 1000)
    {
      cube.display();
      elapsed = millis() - startTime;
    }
  }
}

void Snake::reset()
{
  snake.clear();
  cube.reset();

  // Reset the map matrix
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
  // Disable any active LEDs
  cube.reset();
  
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
  cube.bufferFromMatrix(m);
}

void Snake::logic()
{
  Coord next = { snake.front().x, snake.front().y, snake.front().z };

  // Check if the next location is valid
  switch (io.getDir())
  {
    case xpos:
      next.x + 1 <= 3 ? next.x += 1 : dead = true;
      break;
    case xneg:
      next.x - 1 >= 0 ? next.x -= 1 : dead = true;
      break;
    case ypos:
      next.y + 1 <= 3 ? next.y += 1 : dead = true;
      break;
    case yneg:
      next.y - 1 >= 0 ? next.y -= 1 : dead = true;
      break;
    case zpos:
      next.z + 1 <= 3 ? next.z += 1 : dead = true;
      break;
    case zneg:
      next.z - 1 >= 0 ? next.z -= 1 : dead = true;
      break;
    default:
      dead = true;
      break;
  }
  if (dead) return;

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
  cube.bufferFromMatrix(m);
}

void Snake::generateFood()
{
  // Find an available slot for the food
  do
  {
    food = { random(4), random(4), random(4) };
  } while (m[food.x][food.y][food.z]);
  
  m[food.x][food.y][food.z] = 1;
}
