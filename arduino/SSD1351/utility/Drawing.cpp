#include "utility/Drawing.h"

const Colour Colour::Black	= Colour(0,		0,		0);
const Colour Colour::White	= Colour(255,	255,	255);
const Colour Colour::Red	= Colour(255,	0,		0);
const Colour Colour::Green	= Colour(0,		255,	0);
const Colour Colour::Blue	= Colour(0,		0,		255);
const Colour Colour::Yellow	= Colour(255,	255,	0);

Colour::Colour()
{
	// Default the colour to black.
	this->r = this->g = this->b = 0;
}

Colour::Colour(uint8_t r, uint8_t g, uint8_t b)
{
	this->r = r >> 2;
	this->g = g >> 2;
	this->b = b >> 2;
}

Point::Point()
{
	this->x = 0;
	this->y = 0;
}

Point::Point(int16_t x, int16_t y)
{
	this->x = x;
	this->y = y;
}

Rectangle::Rectangle()
{
	this->width = 0;
	this->height = 0;
}

Rectangle::Rectangle(int16_t x, int16_t y, int16_t w, int16_t h)
{
	this->position = Point(x, y);
	this->width = w;
	this->height = h;
}

Rectangle::Rectangle(Point position, int16_t w, int16_t h)
{
	this->position = position;
	this->width = w;
	this->height = h;
}

int16_t Rectangle::left()
{
	return this->position.x;
}

int16_t Rectangle::right()
{
	return this->position.x + this->width - 1;
}

int16_t Rectangle::top()
{
	return this->position.y;
}

int16_t Rectangle::bottom()
{
	return this->position.y + this->height - 1;
}

Circle::Circle()
{
	this->radius = 0;
}

Circle::Circle(int16_t x, int16_t y, int16_t radius)
{
	this->position = Point(x, y);
	this->radius = radius;
}

Circle::Circle(Point position, int16_t radius)
{
	this->position = position;
	this->radius = radius;
}