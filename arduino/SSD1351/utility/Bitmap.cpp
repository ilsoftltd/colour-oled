/*

  Bitmap.cpp
  
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

#include "utility/Bitmap.h"

Bitmap* Bitmap::LoadBitmap(char *file)
{
	if (!SD.exists(file)) return NULL;

	File image = SD.open(file, O_RDONLY);

	if (image == NULL) return NULL;

	return new Bitmap(image);
}

Bitmap::Bitmap(File file)
{
	// Make sure we have a file.
	if (file == NULL) return;
	
	// Read the header first.
	file.read(&header, sizeof(BitmapHeader));

	// Check to make sure it's a bitmap.
	if (header.magic != 0x4d42)
	{
		file.close();
		return;
	}

	// Now read the DibHeader
	file.read(&dibHeader, sizeof(BitmapDibHeader));

	// Now seek to the beginning of the bitmap data.
	if (!file.seek(header.offset))
	{
		file.close();
		return;
	}

	image = file;
}

Bitmap::~Bitmap()
{

}

uint32_t Bitmap::getWidth()
{
	return dibHeader.width;
}

uint32_t Bitmap::getHeight()
{
	return dibHeader.height;
}

void Bitmap::memsetColour(Colour *buf, size_t size, uint16_t line)
{
	if (image == NULL) return;

	image.seek(image.size() - ((line + 1) * (uint16_t)sizeof(Colour) * getWidth()));
	
	image.read(buf, size * sizeof(Colour));

	reverseBytes(buf, size);
}

void Bitmap::close()
{
	if (image == NULL) return;

	image.close();
}

void Bitmap::reverseBytes(Colour *start, size_t size)
{
    Colour *lo = start;
    Colour *hi = start + size - 1;
    uint8_t swap;

	for (int i = 0; i < size; i++)
	{
		swap = start[i].r;
		start[i].r = start[i].b;
		start[i].b = swap;
	}
}