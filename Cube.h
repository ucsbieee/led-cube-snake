/*
 * File:     Cube.h
 * Author:   Jeff Longo
 * Comments: LED cube controller class
*/

// Header guard, prevents including this file more than once
#ifndef CUBE_H
#define CUBE_H

#include "Arduino.h"

// Begin pin definitions
#define SERIAL_IN  2
#define LATCH_CLK  3
#define INPUT_CLK  4
#define UPDATE_CLK 5
#define RESET      6
#define SET        7
// End pin definitions

class Cube
{
  public:
    // Initializes the cube
    Cube();

    // Adds an LED to the buffer at position (x, y, z) with range (0, size - 1)
    void bufferLED(byte x, byte y, byte z);

    // Parses the data from the buffer, displays the frame, then clears the buffer
    void display();

    // Emptys the contents of the buffer
    void clearBuffer();
    
    // Resets the system
    void reset();
    
  private:
    const static byte size = 4;
    const static int period = 100;
    unsigned short buffer[size];
};

#endif // CUBE_H
