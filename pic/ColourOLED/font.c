#include "font.h"

uint8_t getFontByte(uint8_t offset)
{
	uint8_t val = fontData[offset];
	
	return val;
}

uint16_t getFontWord(uint8_t offset)
{
	uint16_t val = ((uint16_t)fontData[offset] << 8) + (uint16_t)fontData[offset + 1];
	
	return val;
}

glyph_t getGlyph(char c, GLYPH_DATA *glyphData)
{
	rom uint8_t *p = fontData;
	uint8_t fontFormat = getFontFormat();
	uint8_t dataStructureSize = getGlyphStructureSize();
	uint8_t start, end;
	uint16_t pos;
	uint8_t i;
	uint8_t mask = 255;
	
	if (fontFormat == 1) mask = 15;

	start = getFontStartEncoding();
	end = getFontEndEncoding();
	pos = getEncoding97Pos();

	if (c >= 97 && pos > 0)
	{
		printf("Pos: %lu\r\n", pos);
		p += pos;
		start = 97;
	}
	else
	{
		pos = getEncoding65Pos();

		if (c >= 65 && pos > 0)
		{
			p += pos;
			start = 65;
		}
		else
		{
			p += FONT_DATA_STRUCT_SIZE;
		}
	}

	if (c > end)
	{
		*glyphData = getEmptyGlyphData();
		return NULL;
	}

	i = start;

	if (i <= end)
	{
		for (;;)
		{
			if (*p == 255)
			{
				p++;
			}
			else
			{
				if (i == c)
				{
					printf("p is %u\r\n", *p);
					
					*glyphData = getGlyphData(p);
					
					return p;
				}

				p += p[2] & mask;
				p += dataStructureSize;
			}

			if (i == end) break;

			i++;
		}
	}

	*glyphData = getEmptyGlyphData();

	return NULL;
}

uint8_t getFontFormat(void)
{
	return getFontByte(0);
}

uint8_t getBBXWidth(void)
{
	return getFontByte(1);
}

uint8_t getBBXHeight(void)
{
	return getFontByte(2);
}

uint8_t getBBXOffX(void)
{
	return getFontByte(3);
}

uint8_t getBBXOffY(void)
{
	return getFontByte(4);
}

uint8_t getCapitalAHeight(void)
{
	return getFontByte(5);
}

uint16_t getEncoding65Pos(void)
{
	return getFontWord(6);
}

uint16_t getEncoding97Pos(void)
{
	return getFontWord(8);
}

uint8_t getFontStartEncoding(void)
{
	return getFontByte(10);
}

uint8_t getFontEndEncoding(void)
{
	return getFontByte(11);
}

uint8_t getLowerGDesent(void)
{
	return getFontByte(12);
}

uint8_t getAscent(void)
{
	return getFontByte(13);
}

uint8_t getDescent(void)
{
	return getFontByte(14);
}

uint8_t getXAscent(void)
{
	return getFontByte(15);
}

uint8_t getXDescent(void)
{
	return getFontByte(16);
}

uint8_t getGlyphStructureSize()
{
	switch (getFontFormat())
	{
	case 0:
	case 2:
		return 6;

	default:
		return 3;
		break;
	}
}

GLYPH_DATA getGlyphData(rom uint8_t *g)
{
	uint8_t tmp;
	GLYPH_DATA data;
	
	switch (getFontFormat())
	{
	case 0:
	case 2:
		data.gWidth = g[0];
		data.gHeight = g[1];
		data.dx = g[3];
		data.x = g[4];
		data.y = g[5];

		break;

	default:
		tmp = g[0];
		data.y = (tmp & 15) - 2;
		tmp >>= 4;
		data.x = tmp;

		tmp = g[1];
		data.gHeight = tmp & 15;
		tmp >>= 4;
		data.gWidth = tmp;

		tmp = g[2];
		tmp >>= 4;
		data.dx = tmp;

		break;
	}
		
	return data;
	
}

GLYPH_DATA getEmptyGlyphData()
{
	GLYPH_DATA data;

	data.x = data.y = data.gWidth = data.gHeight = data.dx = 0;

	return data;
}

uint8_t *getGlyphDataStart(glyph_t g)
{
	return g + getGlyphStructureSize();
}

void draw8Pixel(PIXEL_DATA *pixelData)
{
	uint8_t data = pixelData->data;
	uint8_t dx = 0;
	uint8_t dy = 0;

	//printf("Data: 0x%02x\r\n", data);

	switch (pixelData->direction)
	{
	case 0: dx++; break;
	case 1: dy++; break;
	case 2: dx--; break;
	case 3: dy--; break;
	}

	do
	{
		if (data & 128) ssd1351_drawPixel(pixelData->position.x, pixelData->position.y, pixelData->colour);
		
		pixelData->position.x += dx;
		pixelData->position.y += dy;

		data <<= 1;
	} while (data != 0);
}

uint8_t fontVrefCalcFont()
{
	return 0;
}

uint8_t fontVrefCalcTop()
{
	return (uint8_t)ascent + 1;
}

uint8_t fontVrefCalcBottom()
{
	return (uint8_t)descent;
}

uint8_t getFontPos()
{
	switch (fontVref)
	{
	case FontPositionTop:
		return fontVrefCalcTop();
	case FontPositionBottom:
		return fontVrefCalcBottom();
	case FontPositionBaseline:
		return fontVrefCalcFont();
	}

	return 0;
}


void updateRefHeights(void)
{
	uint16_t ls;

	switch (fontHeightMode)
	{
	case HeightModeText:
		ascent = getCapitalAHeight();
		descent = getLowerGDesent();
		break;

	case HeightModeXText:
		ascent = getXAscent();
		descent = getXDescent();
		break;

	default:
		ascent = getAscent();
		descent = getDescent();
		break;
	}

	ls = ascent - descent;

	if (lineSpacingFactor != 64)
	{
		ls &= 255;
		ls *= lineSpacingFactor;
		ls >>= 6;
	}

	lineSpacing = ls;
}

int8_t drawGlyph(POINT position, char c, COLOUR colour)
{
	register rom uint8_t *data;
	int16_t w, h;
	GLYPH_DATA gData;
	int16_t ix, iy;
	glyph_t g = getGlyph(c, &gData);

	position.y += getFontPos();

	if (g == NULL) return 0;

	data = getGlyphDataStart(g);
	
	w = gData.gWidth;
	h = gData.gHeight;
	position.x += gData.x;
	position.y -= gData.y;

	w += 7;
	w /= 8;
	iy = position.y;
	iy -= h;
	iy++;

	printf("w: %ld, h: %ld\r\n", w, h);

	for (int j = 0; j < h; j++)
	{
		ix = position.x;

		for (int i = 0; i < w; i++)
		{
			PIXEL_DATA pData;
			pData.position.x = ix;
			pData.position.y = iy;
			pData.colour = colour;
			pData.data = *data;
			pData.direction = 0;

			draw8Pixel(&pData);

			data++;
			ix += 8;
		}

		iy++;
	}

	return gData.dx;
}

void setFontPosBaseline()
{
	fontVref = FontPositionBaseline;
}