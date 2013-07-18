/*

  Bitmap.h
  
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

#ifndef _BITMAP_h
#define _BITMAP_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "utility/defs.h"
#include <SD.h>
#include "utility/Drawing.h"

typedef struct BitmapHeader
{
	uint16_t magic;		// Magic number (Normally BM)
	uint32_t fileSize;
	uint16_t creator1;
	uint16_t creator2;
	uint32_t offset;
} BITMAP_HEADER;

typedef struct BitmapDibHeader
{
	uint32_t headerSize;
	uint32_t width;
	uint32_t height;
	uint16_t nPlanes;
	uint16_t depth;
	uint32_t compressType;
	uint32_t bmpBytes;
	uint32_t hRes;
	uint32_t vRes;
	uint32_t nColours;
	uint32_t nImpColours;
} BITMAP_DIB_HEADER;

class Bitmap
{
private:
	File image;
	BitmapHeader header;
	BitmapDibHeader dibHeader;

	void reverseBytes(Colour *start, size_t size);

public:
	static Bitmap* LoadBitmap(char *file);

	Bitmap(File file);
	~Bitmap();

	uint32_t getWidth();
	uint32_t getHeight();
	void memsetColour(Colour *buf, size_t size, uint16_t line);
	void close();
};

#endif

