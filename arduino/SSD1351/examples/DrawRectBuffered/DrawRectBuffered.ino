/*

  DrawRectBuffered.ino
  
  Colour OLED Breakout Board Library
  
  Copyright (c) 2013, ILSoft Ltd
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  

*/

#include <oledBuffered.h>
#include <SD.h>

// The Arduino Shield from ILSoft uses pins 8, 9 and 7 for CS, DC and Reset.
#define CS		8
#define DC		9
#define RESET	7

// Create a variable to hold our OLED class.
SSD1351 *oled;

void setup()
{
	// Instantiate the class with the pins. 
	oled = new SSD1351(CS, DC, RESET);
}

// Draw the screen.
void draw()
{
	// Draw / fills a rectangle with the colour Red.
	oled->drawRect(Rect(0, 0, 64, 32), Colour::Red);
	oled->fillRect(Rect(64, 32, 64, 32), Colour::Red);
}

void loop()
{
	// Buffer mode requires to reset the line and then loop through all lines and draw.
	oled->firstLine();

	do
	{
		draw();
	} while(oled->nextLine());

	delay(1000);
}
