#ifndef drawing_H
#define drawing_H

#include <stdint.h>

typedef struct Point
{
	int16_t x;
	int16_t y;
} POINT;

typedef struct Rectangle
{
	Point position;
	int16_t width;
	int16_t height;
} RECTANGLE;

typedef struct Circle
{
	Point position;
	int16_t radius;
} CIRCLE;

typedef struct Colour
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} COLOUR;

#endif