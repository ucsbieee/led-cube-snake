/*
 * File:     InputHandler.h
 * Author:   Jeff Longo
 * Comments: Handles inputs from the pushbutton switches
*/

// Header guard, prevents including this file more than once
#ifndef DIRECTION_H
#define DIRECTION_H

enum Direction 
{ 
  xpos, // +x
  xneg, // -x
  ypos, // +y
  yneg, // -y
  zpos, // +z
  zneg  // -z
};

#endif // DIRECTION_H
