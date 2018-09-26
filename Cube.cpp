#include "Cube.h"

Cube::Cube()
{  
  // Configure the necessary pins as output
  pinMode(SERIAL_IN, OUTPUT);
  pinMode(INPUT_CLK, OUTPUT);
  pinMode(LATCH_CLK, OUTPUT);
  pinMode(UPDATE_CLK, OUTPUT);
  pinMode(RESET, OUTPUT);
  pinMode(SET, OUTPUT);
}

void Cube::bufferLED(byte x, byte y, byte z)
{
  // Ensure the specified LED is within the bounds of the cube
  if (x > (size - 1) || y > (size - 1) || z > (size - 1)) return;

  // Convert the x,y position to a mapping of the format used for the buffer
  /* 
   *  (1, 1) --> 1                 -> 1
   *  (1, 2) --> 10                -> 2
   *  (1, 3) --> 100               -> 4
   *  ...
   *  (4, 4) --> 1000000000000000  -> 32768
  */
  unsigned short mapped = pow(2, (x + size*y));
  
  // Add the LED to the buffer
  buffer[z] = buffer[z] | mapped;
}

void Cube::bufferLED(coord c)
{
  // Ensure the specified LED is within the bounds of the cube
  if (c.x > (size - 1) || c.y > (size - 1) || c.z > (size - 1)) return;

  // Convert the x,y position to a mapping of the format used for the buffer
  /* 
   *  (1, 1) --> 1                 -> 1
   *  (1, 2) --> 10                -> 2
   *  (1, 3) --> 100               -> 4
   *  ...
   *  (4, 4) --> 1000000000000000  -> 32768
  */
  unsigned short mapped = pow(2, (size*c.x + c.y));
  
  // Add the LED to the buffer
  buffer[c.z] = buffer[c.z] | mapped;
}

void Cube::reset()
{
  clearBuffer();
  // Clear the layer shift register
  digitalWrite(RESET, LOW);
  delayMicroseconds(period);
  digitalWrite(RESET, HIGH);
  delayMicroseconds(period);
  // Start the layer shift register
  digitalWrite(SET, HIGH);
  delayMicroseconds(period);
  digitalWrite(UPDATE_CLK, HIGH);
  delayMicroseconds(period);
  digitalWrite(UPDATE_CLK, LOW);
  delayMicroseconds(period); // probably not necessary
  digitalWrite(SET, LOW);
}

void Cube::display()
{
  // Update each layer (z-axis)
  for (byte i = 0; i < size; i++)
  {
    // Update x-y plane
    for (byte j = 0; j < size*size; j++)
    {
      // Retrieve the state of bit j on the current layer
      byte curr = (buffer[i] >> (size*size - 1 - j)) &  1;
      // Load in the bit
      digitalWrite(SERIAL_IN, curr);
      // Toggle the input clock to push the bit through
      digitalWrite(INPUT_CLK, HIGH);
      // Provide time for the input to update
      delayMicroseconds(period);
      // Toggle the input clock to wait for the next bit
      digitalWrite(INPUT_CLK, LOW);
      // Provide time to wait for the next bit
      delayMicroseconds(period);
    }
    // Toggle the latch clock to save the output
    digitalWrite(LATCH_CLK, HIGH);
    // Provide time for the output to latch
    delayMicroseconds(period);
    // Toggle the latch clock before loading new data
    digitalWrite(LATCH_CLK, LOW);
    // Provide time before changing the layer
    delayMicroseconds(period);
    // Toggle the update clock to switch to the next layer
    digitalWrite(UPDATE_CLK, HIGH);
    // Provide time to switch layers
    delayMicroseconds(period);
    // Toggle the update clock to work with the selected layer
    digitalWrite(UPDATE_CLK, LOW);
  }
  clearBuffer(); 
}

void Cube::clearBuffer()
{
  for (byte i = 0; i < size; i++) buffer[i] = 0;
}
