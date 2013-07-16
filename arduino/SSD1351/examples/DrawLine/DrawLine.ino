// Uncomment this line for direct line drawing.
#include <oledDirect.h>

// Uncomment this line for buffered drawing.
//#include <oledBuffered.h>

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
	// Draw a line between two points with the colour Red.
	oled->drawLine(Point(0, 0), Point(64, 32), Colour::Red);
}

bool flip = false;

void loop()
{
	// Clear the screen.
	oled->clearScreen(Colour::Black);

	// Change orientation to be rotated 180.
	oled->setOrientation((flip ? CW180 : CW0));
	flip = !flip;

#if SSD1351_MODE == SSD1351_MODE_BUFFER
	// Buffer mode requires to reset the line and then loop through all lines and draw.
	oled->firstLine();

	do
	{
#endif

		draw();

#if SSD1351_MODE == SSD1351_MODE_BUFFER
	} while(oled->nextLine());
#endif

	delay(1000);
}
