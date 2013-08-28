/*

  SSD1351b.cpp
  
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

#include "utility/SSD1351b.h"

SSD1351b::SSD1351b(uint8_t csPin, uint8_t dcPin, uint8_t resetPin)
{
	com = new DisplayCom();
	com->setupPins(csPin, dcPin, resetPin);

	if (com->writeEscSeq(initSeq) == -1) return;

	lineBuf = new Colour[WIDTH];

	if (lineBuf == NULL) return;

	currentLine = 0;
	isInitialised = true;
	
	firstLine();

	do
	{
		fillRect(Rectangle(0, 0, 128, 128), Colour::Black);
	} while (nextLine());
}

SSD1351b::~SSD1351b()
{
	delete com;
}

void SSD1351b::firstLine()
{
	currentLine = 0;
	memset(lineBuf, 0, WIDTH * sizeof(Colour));

	// Send the address.
	com->writeEscSeq(colRowSeq);
}

bool SSD1351b::nextLine()
{
	// Do the draw.
	com->enableChip(true);
	
	com->writeColourBuf(lineBuf, WIDTH);
	
	com->enableChip(false);

	// Clear the mem.
	memset(lineBuf, 0, WIDTH * sizeof(Colour));

	// Increment line.
	currentLine++;

	// Return true if there is more lines.
	return currentLine < HEIGHT;
}

void SSD1351b::setFont(Fontb *font)
{
	if (this->font != font)
	{
		this->font = font;

		if (this->font != NULL)
		{
			this->font->updateRefHeights();
			this->font->setFontPosBaseline();
		}
	}
}

void SSD1351b::clearScreen(Colour colour)
{
	fillRect(Rectangle(0, 0, WIDTH, HEIGHT), colour);
}

void SSD1351b::drawPixel(uint16_t x, uint16_t y, Colour colour)
{
	this->drawPixel(Point(x, y), colour);
}

void SSD1351b::drawPixel(Point pixel, Colour colour)
{
	if (!isInitialised) return;
	if (pixel.x < 0 || pixel.x >= WIDTH) return;
	if (pixel.y < 0 || pixel.y >= HEIGHT) return;
	if (!intersects(pixel.y, 1)) return;

	lineBuf[pixel.x] = colour;
}

void SSD1351b::drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Colour colour)
{
	this->drawLine(Point(x1, y1), Point(x2, y2), colour);
}

void SSD1351b::drawLine(Point start, Point end, Colour colour)
{
	bool steep = abs(end.y - start.y) > abs(end.x - start.x);

	if (!isInitialised) return;

	if (steep)
	{
		swap(&start.x, &start.y);
		swap(&end.x, &end.y);
	}

	if (start.x > end.x)
	{
		swap(&start.x, &end.x);
		swap(&start.y, &end.y);
	}

	int deltaX = end.x - start.x;
	int deltaY = abs(end.y - start.y);
	int err = deltaX >> 1;
	int yStep = (start.y < end.y ? 1 : -1);
	int y = start.y;

	com->enableChip(true);

	for (int x = start.x; x <= end.x; x++)
	{
		if (steep)
		{
			drawPixel(Point(y, x), colour);
		}
		else
		{
			drawPixel(Point(x, y), colour);
		}

		err -= deltaY;

		if (err < 0)
		{
			y += yStep;
			err += deltaX;
		}
	}

	com->enableChip(false);
}

void SSD1351b::drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Colour colour)
{
	this->drawRect(Rectangle(x, y, w, h), colour);
}

void SSD1351b::drawRect(Rectangle rect, Colour colour)
{
	if (!isInitialised) return;

	com->enableChip(true);

	drawLine(Point(rect.left(), rect.top()),	Point(rect.right(), rect.top()),	colour);	// Top
	drawLine(Point(rect.left(), rect.bottom()), Point(rect.right(), rect.bottom()), colour);	// Bottom
	drawLine(Point(rect.left(), rect.top()),	Point(rect.left(), rect.bottom()),	colour);	// Left
	drawLine(Point(rect.right(), rect.top()),	Point(rect.right(), rect.bottom()),	colour);	// Right

	com->enableChip(false);
}

void SSD1351b::fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Colour colour)
{
	this->fillRect(Rectangle(x, y, w, h), colour);
}

void SSD1351b::fillRect(Rectangle rect, Colour colour)
{
	if (!isInitialised) return;
	if (!intersects(rect.position.y, rect.height)) return;

	int width = rect.width;

	if (rect.position.x + width > WIDTH) width = WIDTH - rect.position.x;

	// We store a line buffer here just to speed up the drawing.
	for (int16_t cX = 0; cX < width; cX++)
	{
		lineBuf[rect.left() + cX] = colour;
	}
}

void SSD1351b::drawCircle(uint16_t x, uint16_t y, uint16_t radius, Colour colour)
{
	this->drawCircle(Circle(x, y, radius), colour);
}

void SSD1351b::drawCircle(Circle circle, Colour colour)
{
	int16_t x, y;
	int16_t centreX = circle.position.x;
	int16_t centreY = circle.position.y;
	int calc;

	x = circle.radius;
	y = 0;
	calc = x;

	while (x >= y)
	{
		drawPixel(Point(centreX + x, centreY + y), colour);
		drawPixel(Point(centreX + x, centreY - y), colour);
		drawPixel(Point(centreX - x, centreY + y), colour);
		drawPixel(Point(centreX - x, centreY - y), colour);
		drawPixel(Point(centreX + y, centreY + x), colour);
		drawPixel(Point(centreX + y, centreY - x), colour);
		drawPixel(Point(centreX - y, centreY + x), colour);
		drawPixel(Point(centreX - y, centreY - x), colour);

		++y;
		calc -= 2 * y - 1;

		if (calc <= 0)
		{
			--x;
			calc += 2 * x + 1;
		}
	}
}

void SSD1351b::fillCircle(uint16_t x, uint16_t y, uint16_t radius, Colour colour)
{
	this->fillCircle(Circle(x, y, radius), colour);
}

void SSD1351b::fillCircle(Circle circle, Colour colour)
{
	if (!isInitialised) return;

	int16_t x, y, xT, d;
	int calc;

	x = circle.radius;
	y = 0;
	calc = x;

	com->enableChip(true);
	
	while (x >= y)
	{
		if (x <= circle.position.x)
		{
			xT = circle.position.x - x;
			d = 2 * x;
		}
		else
		{
			xT = 0;
			d = x + circle.position.x;
		}

		fillRect(Rectangle(xT, circle.position.y + y, d, 1), colour);
		fillRect(Rectangle(xT, circle.position.y - y, d, 1), colour);

		if (y <= circle.position.x)
		{
			xT = circle.position.x - y;
			d = 2 * y;
		}
		else
		{
			xT = 0;
			d = y + circle.position.x;
		}

		fillRect(Rectangle(xT, circle.position.y + x, d, 1), colour);
		fillRect(Rectangle(xT, circle.position.y - x, d, 1), colour);

		++y;
		calc -= 2 * y - 1;

		if (calc <= 0)
		{
			--x;
			calc += 2 * x + 1;
		}
	}

	com->enableChip(false);
}

void SSD1351b::drawPolygon(Point *points, int length, Colour colour)
{
	if (!isInitialised) return;

	for (int i = 0; i < length - 1; i++)
	{
		drawLine(points[i], points[i + 1], colour);
	}

	// Now connect the last line to the first.
	drawLine(points[length - 1], points[0], colour);
}

void SSD1351b::drawString(uint16_t x, uint16_t y, char* str, Colour colour)
{
	this->drawString(Point(x, y), str, colour);
}

void SSD1351b::drawString(Point position, char* str, Colour colour)
{
	if (font == NULL || !isInitialised) return;

	int8_t d;

	while (*str != '\0')
	{
		d = font->drawGlyph(this, position, *str, colour);
		position.x += d;
		str++;
	}
}

void SSD1351b::drawBitmap(uint16_t x, uint16_t y, Bitmap *bitmap)
{
	this->drawBitmap(Point(x, y), bitmap);
}

void SSD1351b::drawBitmap(Point position, Bitmap *bitmap)
{
	if (bitmap == NULL) return;

	int width = bitmap->getWidth();
	int height = bitmap->getHeight();
	if (position.x + width > WIDTH) width = WIDTH - position.x;
	if (position.y + height > HEIGHT) height = HEIGHT - position.y;

	if (!intersects(position.y, height)) return;

	bitmap->memsetColour(lineBuf + position.x, width, currentLine - position.y);
}

void SSD1351b::setOrientation(Orientation orientation)
{
	if (!isInitialised) return;

	com->enableChip(true);

	com->setData(false);
	
#ifdef __AVR__
	com->writeData(0xa0);
#else
	com->writeData(0xa0, true);
#endif

	com->setData(true);
	
	switch (orientation)
	{
	case CW0:
#ifdef __AVR__
		com->writeData(0xb4);
#else
		com->writeData(0xb0, false);
#endif
		break;

	case CW90:
#ifdef __AVR__
		com->writeData(0xb7);
#else
		com->writeData(0xb7, false);
#endif
		break;

	case CW180:
#ifdef __AVR__
		com->writeData(0xa6);
#else
		com->writeData(0xa6, false);
#endif
		break;

	case CW270:
#ifdef __AVR__
		com->writeData(0xa5);
#else
		com->writeData(0xa5, false);
#endif
		break;

	default:
		break;
	}

	com->enableChip(false);
}

void SSD1351b::setInvert(bool inverted)
{
	if (!isInitialised) return;

	com->enableChip(true);

	com->setData(false);

	if (inverted)
	{
#ifdef __AVR__
		com->writeData(0xa7);
#else
		com->writeData(0xa7, true);
#endif
	}
	else
	{
#ifdef __AVR__
		com->writeData(0xa6);
#else
		com->writeData(0xa6, true);
#endif
	}

#ifdef __AVR__
		com->writeData(0x5c);
#else
		com->writeData(0x5c, false);
#endif

	com->setData(true);
	com->enableChip(false);
}

void SSD1351b::screenOn()
{
	if (!isInitialised) return;

	com->enableChip(true);

	com->setData(false);

#ifdef __AVR__
	com->writeData(0xaf);
	com->writeData(0x5c);
#else
	com->writeData(0xaf, true);
	com->writeData(0x5c, false);
#endif

	com->setData(true);
	com->enableChip(false);
}

void SSD1351b::screenOff()
{
	if (!isInitialised) return;

	com->enableChip(true);

	com->setData(false);
	
#ifdef __AVR__
	com->writeData(0xae);
	com->writeData(0x5c);
#else
	com->writeData(0xae, true);
	com->writeData(0x5c, false);
#endif
	
	com->setData(true);
	com->enableChip(false);
}

void SSD1351b::setContrast(uint8_t contrast)
{
	if (!isInitialised) return;

	if (contrast > 16) contrast = 16;

	com->enableChip(true);
	com->setData(false);
	
#ifdef __AVR__
	com->writeData(0xc7);
#else
	com->writeData(0xc7, true);
#endif

	com->setData(true);
	
#ifdef __AVR__
	com->writeData(contrast);
#else
	com->writeData(contrast, true);
#endif

	com->setData(false);

#ifdef __AVR__
	com->writeData(0x5c);
#else
	com->writeData(0x5c, false);
#endif

	com->setData(true);
	com->enableChip(false);
}

inline void SSD1351b::swap(int16_t *x, int16_t *y)
{
	uint16_t t = *x;

	*x = *y;
	*y = t;
}

inline bool SSD1351b::intersects(int16_t y, int16_t h)
{
	return ((currentLine >= y) && ( currentLine < y + h));
}