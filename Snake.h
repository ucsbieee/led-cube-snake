/*
 * File:     InputHandler.h
 * Author:   Jeff Longo
 * Comments: Snake game implementation
*/

// Header guard, prevents including this file more than once
#ifndef SNAKE_H
#define SNAKE_H

#include <ArduinoSTL.h>
#include <deque>
#include "Cube.h"
#include "InputHandler.h"
#include "Direction.h"

class Snake
{
  public:
    // Initializes the snake
    Snake();
    
    // Polls for a start button press then initializes runtime variables
    void start();

    // Executes one game tick
    void update();

    // Resets the game state
    void reset();

    // Checks if the snake is alive
    bool isDead() { return dead; }
    
  private:
    // Structure to hold a 3D coordinate point
    struct Coord { byte x, y, z; };

    // Initializes runtime variables
    void init();

    // Executes game logic
    void logic();

    // Generates a new food piece
    void generateFood();

    // Loads map data to the LED cube
    void bufferFromMatrix();

    Cube cube;
    bool m[4][4][4];
    std::deque<Coord> snake;
    InputHandler io;
    bool dead = false;
    Direction dir, prevDir;
    Coord food;
    int frameTime;
};

#endif // SNAKE_H
