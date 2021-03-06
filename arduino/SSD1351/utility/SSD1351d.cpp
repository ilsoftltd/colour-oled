/*

  SSD1351d.cpp
  
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

#include "utility/SSD1351d.h"

SSD1351d::SSD1351d(uint8_t csPin, uint8_t dcPin, uint8_t resetPin)
{
	com = new DisplayCom();
	com->setupPins(csPin, dcPin, resetPin);

	if (com->writeEscSeq(initSeq) == -1) return;

	isInitialised = true;
	
	fillRect(Rectangle(0, 0, 128, 128), Colour::Black);
}

SSD1351d::~SSD1351d()
{
	delete com;
}

void SSD1351d::setFont(Fontd *font)
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

void SSD1351d::clearScreen(Colour colour)
{
	fillRect(Rectangle(0, 0, WIDTH, HEIGHT), colour);
}

void SSD1351d::drawPixel(uint16_t x, uint16_t y, Colour colour)
{
	this->drawPixel(Point(x, y), colour);
}

void SSD1351d::drawPixel(Point pixel, Colour colour)
{
	if (!isInitialised) return;
	if (pixel.x < 0 || pixel.x >= WIDTH) return;
	if (pixel.y < 0 || pixel.y >= HEIGHT) return;

	com->enableChip(true);
	colRowSeq(Rectangle(pixel, 1, 1));
	
#ifdef __AVR__
	com->writeColour(colour);
#else
	com->writeColour(colour, false);
#endif

	com->enableChip(false);
}

void SSD1351d::drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Colour colour)
{
	this->drawLine(Point(x1, y1), Point(x2, y2), colour);
}

void SSD1351d::drawLine(Point start, Point end, Colour colour)
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
			drawPixelNoCS(Point(y, x), colour);
		}
		else
		{
			drawPixelNoCS(Point(x, y), colour);
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

void SSD1351d::drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Colour colour)
{
	this->drawRect(Rectangle(x, y, w, h), colour);
}

void SSD1351d::drawRect(Rectangle rect, Colour colour)
{
	if (!isInitialised) return;

	com->enableChip(true);

	drawLine(Point(rect.left(), rect.top()),	Point(rect.right(), rect.top()),	colour);	// Top
	drawLine(Point(rect.left(), rect.bottom()), Point(rect.right(), rect.bottom()), colour);	// Bottom
	drawLine(Point(rect.left(), rect.top()),	Point(rect.left(), rect.bottom()),	colour);	// Left
	drawLine(Point(rect.right(), rect.top()),	Point(rect.right(), rect.bottom()),	colour);	// Right

	com->enableChip(false);
}

void SSD1351d::fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Colour colour)
{
	this->fillRect(Rectangle(x, y, w, h), colour);
}

void SSD1351d::fillRect(Rectangle rect, Colour colour)
{
	if (!isInitialised) return;

	int width = rect.width;
	int height = rect.height;

	if (rect.position.x + width > WIDTH) width = WIDTH - rect.position.x;
	if (rect.position.y + height > HEIGHT) height = HEIGHT - rect.position.y;

	size_t size = width;
	Colour *col = new Colour[size];

	com->enableChip(true);
	colRowSeq(rect);

	// We store a line buffer here just to speed up the drawing.
	for (int16_t cX = 0; cX < width; cX++)
	{
		col[cX] = colour;
	}

	for (int16_t cY = 0; cY < height; cY++)
	{
		com->writeColourBuf(col, size);
	}

	delete col;
	com->enableChip(false);
}

void SSD1351d::drawCircle(uint16_t x, uint16_t y, uint16_t radius, Colour colour)
{
	this->drawCircle(Circle(x, y, radius), colour);
}

void SSD1351d::drawCircle(Circle circle, Colour colour)
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

void SSD1351d::fillCircle(uint16_t x, uint16_t y, uint16_t radius, Colour colour)
{
	this->fillCircle(Circle(x, y, radius), colour);
}

void SSD1351d::fillCircle(Circle circle, Colour colour)
{
	if (!isInitialised) return;

	int16_t x, y, xT, d;
	int calc;

	x = circle.radius;
	y = 0;
	calc = x;

	com->enableChip(true);
	colRowSeq(Rectangle(circle.position, circle.radius, circle.radius));

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

void SSD1351d::drawPolygon(Point *points, int length, Colour colour)
{
	if (!isInitialised) return;

	for (int i = 0; i < length - 1; i++)
	{
		drawLine(points[i], points[i + 1], colour);
	}

	// Now connect the last line to the first.
	drawLine(points[length - 1], points[0], colour);
}

void SSD1351d::drawString(uint16_t x, uint16_t y, char* str, Colour colour)
{
	this->drawString(Point(x, y), str, colour);
}

void SSD1351d::drawString(Point position, char* str, Colour colour)
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

void SSD1351d::drawBitmap(uint16_t x, uint16_t y, Bitmap *bitmap)
{
	this->drawBitmap(Point(x, y), bitmap);
}

void SSD1351d::drawBitmap(Point position, Bitmap *bitmap)
{
	if (bitmap == NULL) return;

	int width = bitmap->getWidth();
	int height = bitmap->getHeight();

	if (position.x + width > WIDTH) width = WIDTH - position.x;
	if (position.y + height > HEIGHT) height = HEIGHT - position.y;

	Colour *lineBuf = new Colour[width];

	com->enableChip(true);
	colRowSeq(Rectangle(position, width, height));
	com->enableChip(false);

	for (int i = position.y; i < position.y + height; i++)
	{
		bitmap->memsetColour(lineBuf, width, i - position.y);

		com->enableChip(true);
		com->writeColourBuf(lineBuf, width);
		com->enableChip(false);
	}

	delete lineBuf;
}

void SSD1351d::setOrientation(Orientation orientation)
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

	case CW180:
#ifdef __AVR__
		com->writeData(0xa6);
#else
		com->writeData(0xa6, false);
#endif
		break;

	default:
		break;
	}

	com->enableChip(false);
}

void SSD1351d::setInvert(bool inverted)
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

void SSD1351d::screenOn()
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

void SSD1351d::screenOff()
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

void SSD1351d::setContrast(uint8_t contrast)
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

inline void SSD1351d::drawPixelNoCS(Point pixel, Colour colour)
{
	if (!isInitialised) return;
	if (pixel.x < 0 || pixel.x >= WIDTH) return;
	if (pixel.y < 0 || pixel.y >= HEIGHT) return;

	colRowSeq(Rectangle(pixel, 1, 1));
	
#ifdef __AVR__
	com->writeColour(colour);
#else
	com->writeColour(colour, false);
#endif
}

inline void SSD1351d::drawPixelNoSeq(Point pixel, Colour colour)
{
	if (!isInitialised) return;
	if (pixel.x < 0 || pixel.x >= WIDTH) return;
	if (pixel.y < 0 || pixel.y >= HEIGHT) return;

#ifdef __AVR__
	com->writeColour(colour);
#else
	com->writeColour(colour, false);
#endif
}

inline void SSD1351d::swap(int16_t *x, int16_t *y)
{
	uint16_t t = *x;

	*x = *y;
	*y = t;
}

void SSD1351d::colRowSeq(Rectangle rect)
{
	com->setData(false);
	
#ifdef __AVR__
	com->writeData(0x15);
#else
	com->writeData(0x15, true);
#endif

	com->setData(true);

#ifdef __AVR__
	com->writeData(rect.left());
	com->writeData(rect.right());
#else
	com->writeData(rect.left(), true);
	com->writeData(rect.right(), true);
#endif

	com->setData(false);
	
#ifdef __AVR__
	com->writeData(0x75);
#else
	com->writeData(0x75, true);
#endif
	
	com->setData(true);

#ifdef __AVR__
	com->writeData(rect.top());
	com->writeData(rect.bottom());
#else
	com->writeData(rect.top(), true);
	com->writeData(rect.bottom(), true);
#endif

	com->setData(false);

#ifdef __AVR__
	com->writeData(0x5c);
#else
	com->writeData(0x5c, false);
#endif
	
	com->setData(true);
}