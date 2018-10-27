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
   *  (0, 0) --> 000000000000001   -> 1
   *  (1, 0) --> 000000000000010   -> 2
   *  (2, 0) --> 000000000000100   -> 4
   *  ...
   *  (3, 3) --> 1000000000000000  -> 32768
  */
  unsigned short mapped = pow(2, (x + size*y)) + 0.5;
  
  // Add the LED to the buffer
  buffer[z] = buffer[z] | mapped;
}

void Cube::bufferFromMatrix(bool m[size][size][size])
{
  for (byte x = 0; x < 4; x++)
  {
    for (byte y = 0; y < 4; y++)
    {
      for (byte z = 0; z < 4; z++)
      {
        if (m[x][y][z])
        {
          bufferLED(x, y, z);
        }
      }
    }
  }
}

void Cube::toggleLED(byte x, byte y, byte z)
{
   buffer[z] ^= 1 << (x + size*y);
}

void Cube::clearBuffer()
{
  for (byte i = 0; i < size; i++) buffer[i] = 0;
}

void Cube::reset()
{
  clearBuffer();
  // Remove any active LEDs
  display();
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
}
