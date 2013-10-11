#include <iostream>
#include <oledBuffered.h>

using namespace std;

#define SPI_CHANNEL	1 			// The SPI Channel. Pi has 2
#define CS			10			// CS Pin
#define DC			6			// DC Pin
#define RESET		5			// Reset Pin
#define SPEED		20000000	// Speed of the SPI, this uses 20Mhz

SSD1351 *oled;

void setup();
void loop();

int main()
{
	setup();
	
	while (true)
	{
		loop();
	}

	return 0;
}

void setup()
{
// Instantiate the class with the pins
oled = new SSD1351(CHANNEL, CS, DC, RESET, SPEED);
}

// Draw the screen
void draw()
{
	// Draw / fills a circle with the colour Red
	oled->drawCircle(Circle(16,16,32), Colour::Red);
	oled->fillCircle(Circle(64,64,32), Colour::Red);
}

void loop()
{
	//Buffer mode requires to reset the line and then loop through all lines and draw.
	oled->firstLine();

	do
	{
		draw();
	} while(oled->nextLine());

	delay(100);
}