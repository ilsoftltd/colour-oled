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

	image.seek(image.size() - ((line + 1) * (uint16_t)sizeof(Colour) * size));
	
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

		start[i].r >>= 2;
		start[i].g >>= 2;
		start[i].b >>= 2;
	}

    /*while (lo < hi)
	{
	    swap = *lo;
        *lo++ = *hi;
        *hi-- = swap;
    }*/
}