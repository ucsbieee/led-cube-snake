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

  // Start the layer shift register from a known state
  reset();
}

void Cube::bufferLED(byte x, byte y, byte z)
{
  // Ensure the specified LED is within the bounds of the cube
  if (x < 1 || x > size || y < 1 || y > size || z < 1 || z > size) return;

  // Convert the x,y position to a mapping of the format used for the buffer
  /* 
   *  (1, 1) --> 1
   *  (1, 2) --> 10
   *  (1, 3) --> 100
   *  ...
   *  (4, 4) --> 1000000000000000
  */
  unsigned char mapped = pow(2, (size*(x - 1) + (y - 1)));
  
  // Add the LED to the buffer
  buffer[z - 1] = buffer[z - 1] | mapped;
}

void Cube::display()
{
  // Display the contents of the buffer
  update(buffer);
  // Reset the buffer
  for (byte i = 0; i < size; i++) buffer[i] = 0;
}

void Cube::reset()
{
  // Reset the buffer
  for (byte i = 0; i < size; i++) buffer[i] = 0;
  
  // Clear the layer shift register
  digitalWrite(RESET, LOW);
  delay(period);
  digitalWrite(RESET, HIGH);
  delay(period);
  // Start the layer shift register
  digitalWrite(SET, HIGH);
  delay(period);
  digitalWrite(UPDATE_CLK, HIGH);
  delay(period);
  digitalWrite(UPDATE_CLK, LOW);
  delay(period);
  digitalWrite(SET, LOW);
}

void Cube::update(unsigned char data[])
{
  // Update each layer (z-axis)
  for (byte i = 0; i < size; i++)
  {
    // Update x-y plane
    for (byte j = 0; j < size*size; j++)
    {
      // Retrieve the state of bit j on the current layer
      unsigned char curr = (data[i] >> (size*size - 1 - j)) &  1;
      // Load in the bit
      digitalWrite(SERIAL_IN, curr);
      // Toggle the input clock to push the bit through
      digitalWrite(INPUT_CLK, HIGH);
      // Provide time for the input to update
      delay(period);
      // Toggle the input clock to wait for the next bit
      digitalWrite(INPUT_CLK, LOW);
      // Provide time to wait for the next bit
      delay(period);
    }
    // Toggle the latch clock to save the output
    digitalWrite(LATCH_CLK, HIGH);
    // Provide time for the output to latch
    delay(period);
    // Toggle the latch clock before loading new data
    digitalWrite(LATCH_CLK, LOW);
    // Provide time before changing the layer
    delay(period);
    // Toggle the update clock to switch to the next layer
    digitalWrite(UPDATE_CLK, HIGH);
    // Provide time to switch layers
    delay(period);
    // Toggle the update clock to work with the selected layer
    digitalWrite(UPDATE_CLK, LOW);
  }
}

