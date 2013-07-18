/*

  Drawing.h
  
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

#ifndef _DRAWING_h
#define _DRAWING_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "utility/defs.h"

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

typedef Colour Color;

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

