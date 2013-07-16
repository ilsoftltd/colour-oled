// Drawing.h

#ifndef _DRAWING_h
#define _DRAWING_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

typedef struct Colour
{
private:

public:
	uint8_t r;
	uint8_t g;
	uint8_t b;

	Colour();
	Colour(uint8_t r, uint8_t g, uint8_t b);

	static const Colour Black;
	static const Colour White;
	static const Colour Red;
	static const Colour Green;
	static const Colour Blue;
	static const Colour Yellow;
};

typedef struct Point
{
	int16_t x;
	int16_t y;

	Point();
	Point(int16_t x, int16_t y);

	void operator +=(const Point rhs) { x += rhs.x; y += rhs.y; }
	Point operator *(const float rhs) { return Point((int)(((float)x * rhs) + 0.5f), (int)(((float)y * rhs) + 0.5f)); }
};

typedef struct Rectangle
{
	Point position;
	int16_t width;
	int16_t height;

	Rectangle();
	Rectangle(int16_t x, int16_t y, int16_t w, int16_t h);
	Rectangle(Point position, int16_t w, int16_t h);

	int16_t left();
	int16_t right();
	int16_t top();
	int16_t bottom();
} RECTANGLE;

typedef struct Circle
{
	Point position;
	int16_t radius;
	Circle();
	Circle(int16_t x, int16_t y, int16_t radius);
	Circle(Point position, int16_t radius);
} CIRCLE;

#endif

