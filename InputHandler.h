/*
 * File:     InputHandler.h
 * Author:   Jeff Longo
 * Comments: Handles inputs from the pushbutton switches
*/

// Header guard, prevents including this file more than once
#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "Arduino.h"
#include "Direction.h"

#define START 8
#define UP    9
#define DOWN  10
#define LEFT  11
#define RIGHT 12

class InputHandler
{
  public:
    // Initializes the input handler
    InputHandler();

    // Initializes runtime variables
    void init();
    
    // Waits for a start button press
    void pollForStart();

    // Polls for input and handles the snake's direction
    void update();

    // Updates prevDir for the next frame
    void saveDir();
    
    // Set the current direction
    void setDir(Direction dir) { this->dir = dir; }

    // Get the current direction
    Direction getDir(){ return dir; }

  private:
    Direction dir, prevDir;
    bool released;
    unsigned long timeOfUpRelease, timeOfDownRelease;
  
};

#endif // INPUT_HANDLER_H
