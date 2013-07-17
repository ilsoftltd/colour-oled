// Bitmap.h

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

