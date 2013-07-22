/*

  Drawing.cpp
  
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

#include "Drawing.h"

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
	this->r = r;
	this->g = g;
	this->b = b;
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