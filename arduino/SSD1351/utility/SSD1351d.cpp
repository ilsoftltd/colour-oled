#include "utility/SSD1351d.h"

SSD1351::SSD1351(uint8_t csPin, uint8_t dcPin, uint8_t resetPin)
{
	com = new DisplayCom();
	com->setupPins(csPin, dcPin, resetPin);

	if (com->writeEscSeq(initSeq) == -1) return;

	isInitialised = true;
	
	fillRect(Rectangle(0, 0, 128, 128), Colour::Black);
}

SSD1351::~SSD1351()
{
	delete com;
}

void SSD1351::setFont(Font *font)
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

void SSD1351::clearScreen(Colour colour)
{
	fillRect(Rectangle(0, 0, WIDTH, HEIGHT), colour);
}

void SSD1351::drawPixel(Point pixel, Colour colour)
{
	if (!isInitialised) return;
	if (pixel.x < 0 || pixel.x > WIDTH) return;
	if (pixel.y < 0 || pixel.y > HEIGHT) return;

	com->enableChip(true);
	colRowSeq(Rectangle(pixel, 1, 1));
	com->writeColour(colour);
	com->enableChip(false);
}

void SSD1351::drawLine(Point start, Point end, Colour colour)
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

void SSD1351::drawRect(Rectangle rect, Colour colour)
{
	if (!isInitialised) return;

	com->enableChip(true);

	drawLine(Point(rect.left(), rect.top()),	Point(rect.right(), rect.top()),	colour);	// Top
	drawLine(Point(rect.left(), rect.bottom()), Point(rect.right(), rect.bottom()), colour);	// Bottom
	drawLine(Point(rect.left(), rect.top()),	Point(rect.left(), rect.bottom()),	colour);	// Left
	drawLine(Point(rect.right(), rect.top()),	Point(rect.right(), rect.bottom()),	colour);	// Right

	com->enableChip(false);
}

void SSD1351::fillRect(Rectangle rect, Colour colour)
{
	if (!isInitialised) return;

	int i = 0;
	int width = rect.width;
	int height = rect.height;

	if (rect.position.x + width > WIDTH) width = WIDTH - rect.position.x;
	if (rect.position.y + height > HEIGHT) height = HEIGHT - rect.position.y;

	size_t size = width * 3;
	uint8_t *col = new uint8_t[size * 3];

	com->enableChip(true);
	colRowSeq(rect);

	// We store a line buffer here just to speed up the drawing.
	for (int16_t cX = 0; cX < width; cX++)
	{
		col[i++] = colour.r;
		col[i++] = colour.g;
		col[i++] = colour.b;
	}

	for (int16_t cY = 0; cY < height; cY++)
	{
		com->writeDataArray(col, size);
	}

	delete col;
	com->enableChip(false);
}

void SSD1351::drawCircle(Circle circle, Colour colour)
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

void SSD1351::fillCircle(Circle circle, Colour colour)
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

void SSD1351::drawPolygon(Point *points, int length, Colour colour)
{
	if (!isInitialised) return;

	for (int i = 0; i < length - 1; i++)
	{
		drawLine(points[i], points[i + 1], colour);
	}

	// Now connect the last line to the first.
	drawLine(points[length - 1], points[0], colour);
}

void SSD1351::drawString(Point position, char* str, Colour colour)
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

void SSD1351::setOrientation(Orientation orientation)
{
	if (!isInitialised) return;

	com->enableChip(true);

	com->setData(false);
	com->writeData(0xa0);

	com->setData(true);
	
	if (orientation == CW0)
	{
		com->writeData(0xb4);
	}
	else
	{
		com->writeData(0xa6);
	}

	com->enableChip(true);
}

void SSD1351::setInvert(bool inverted)
{
	if (!isInitialised) return;

	com->enableChip(true);

	com->setData(false);

	if (inverted)
	{
		com->writeData(0xa7);
	}
	else
	{
		com->writeData(0xa6);
	}

	com->writeData(0x5c);

	com->setData(true);
	com->enableChip(true);
}

void SSD1351::screenOn()
{
	if (!isInitialised) return;

	com->enableChip(true);

	com->setData(false);
	com->writeData(0xaf);
	com->writeData(0x5c);

	com->setData(true);
	com->enableChip(true);
}

void SSD1351::screenOff()
{
	if (!isInitialised) return;

	com->enableChip(true);

	com->setData(false);
	com->writeData(0xae);
	com->writeData(0x5c);

	com->setData(true);
	com->enableChip(true);
}

void SSD1351::setContrast(uint8_t contrast)
{
	if (!isInitialised) return;

	if (contrast > 16) contrast = 16;

	com->enableChip(true);
	com->setData(false);
	com->writeData(0xc7);

	com->setData(true);
	com->writeData(contrast);

	com->setData(false);
	com->writeData(0x5c);

	com->setData(true);
	com->enableChip(true);
}

inline void SSD1351::drawPixelNoCS(Point pixel, Colour colour)
{
	if (!isInitialised) return;
	if (pixel.x < 0 || pixel.x > WIDTH - 1) return;
	if (pixel.y < 0 || pixel.y > HEIGHT - 1) return;

	colRowSeq(Rectangle(pixel, 1, 1));
	com->writeColour(colour);
}

inline void SSD1351::drawPixelNoSeq(Point pixel, Colour colour)
{
	if (!isInitialised) return;
	if (pixel.x < 0 || pixel.x > WIDTH - 1) return;
	if (pixel.y < 0 || pixel.y > HEIGHT - 1) return;

	com->writeColour(colour);
}

inline void SSD1351::swap(int16_t *x, int16_t *y)
{
	uint16_t t = *x;

	*x = *y;
	*y = t;
}

void SSD1351::colRowSeq(Rectangle rect)
{
	com->setData(false);
	com->writeData(0x15);
	com->setData(true);
	com->writeData(rect.left());
	com->writeData(rect.right());

	com->setData(false);
	com->writeData(0x75);
	com->setData(true);
	com->writeData(rect.top());
	com->writeData(rect.bottom());

	com->setData(false);
	com->writeData(0x5c);
	com->setData(true);
}