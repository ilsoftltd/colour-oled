#include "utility/Fontd.h"

Font::Font(prog_uint8_t *fontData)
{
	ascent = 0;
	descent = 0;
	lineSpacing = 0;
	lineSpacingFactor = 64;

	this->fontData = fontData;
}

Font::~Font(void)
{
}

#pragma region Private Methods

uint8_t Font::getFontByte(uint8_t offset)
{
	return pgm_read_byte(fontData + offset);
}

uint16_t Font::getFontWord(uint8_t offset)
{
	return ((uint16_t)pgm_read_byte(fontData + offset) << 8) + (uint16_t)pgm_read_byte(fontData + offset + 1);
}

glyph_t Font::getGlyph(char c, GlyphData *glyphData)
{
	prog_uint8_t *p = fontData;
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
		//Serial.print("pos: ");
		//Serial.println(pos);

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
			if (pgm_read_byte(p) == 255)
			{
				p++;
			}
			else
			{
				if (i == c)
				{
					*glyphData = getGlyphData(p);

					return p;
				}

				p += pgm_read_byte(p + 2) & mask;
				p += dataStructureSize;
			}

			if (i == end) break;

			i++;
		}
	}

	*glyphData = getEmptyGlyphData();

	return NULL;
}

uint8_t Font::getFontFormat(void)
{
	return getFontByte(0);
}

uint8_t Font::getBBXWidth(void)
{
	return getFontByte(1);
}

uint8_t Font::getBBXHeight(void)
{
	return getFontByte(2);
}

uint8_t Font::getBBXOffX(void)
{
	return getFontByte(3);
}

uint8_t Font::getBBXOffY(void)
{
	return getFontByte(4);
}

uint8_t Font::getCapitalAHeight(void)
{
	return getFontByte(5);
}

uint16_t Font::getEncoding65Pos(void)
{
	return getFontWord(6);
}

uint16_t Font::getEncoding97Pos(void)
{
	return getFontWord(8);
}

uint8_t Font::getFontStartEncoding(void)
{
	return getFontByte(10);
}

uint8_t Font::getFontEndEncoding(void)
{
	return getFontByte(11);
}

uint8_t Font::getLowerGDesent(void)
{
	return getFontByte(12);
}

uint8_t Font::getAscent(void)
{
	return getFontByte(13);
}

uint8_t Font::getDescent(void)
{
	return getFontByte(14);
}

uint8_t Font::getXAscent(void)
{
	return getFontByte(15);
}

uint8_t Font::getXDescent(void)
{
	return getFontByte(16);
}

uint8_t Font::getGlyphStructureSize()
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

GlyphData Font::getGlyphData(glyph_t g)
{
	uint8_t tmp;
	GlyphData data;

	switch (getFontFormat())
	{
	case 0:
	case 2:
		data.width = pgm_read_byte((uint8_t *)g);
		data.height = pgm_read_byte((uint8_t *)g + 1);
		data.dx = pgm_read_byte((uint8_t *)g + 3);
		data.x = pgm_read_byte((uint8_t *)g + 4);
		data.y = pgm_read_byte((uint8_t *)g + 5);

		break;

	default:
		tmp = pgm_read_byte((uint8_t *)g);
		data.y = (tmp & 15) - 2;
		tmp >>= 4;
		data.x = tmp;

		tmp = pgm_read_byte((uint8_t *)g + 1);
		data.height = tmp & 15;
		tmp >>= 4;
		data.width = tmp;

		tmp = pgm_read_byte((uint8_t *)g + 2);
		tmp >>= 4;
		data.dx = tmp;

		break;
	}

	return data;
}

GlyphData Font::getEmptyGlyphData()
{
	GlyphData data;

	data.x = data.y = data.width = data.height = data.dx = 0;

	return data;
}

prog_uint8_t *Font::getGlyphDataStart(glyph_t g)
{
	return g + getGlyphStructureSize();
}

void Font::draw8Pixel(SSD1351 *oled, PixelData *pixelData)
{
	register prog_uint8_t data = pixelData->data;
	uint8_t dx = 0;
	uint8_t dy = 0;

	switch (pixelData->direction)
	{
	case 0: dx++; break;
	case 1: dy++; break;
	case 2: dx--; break;
	case 3: dy--; break;
	}

	do
	{
		if (data & 128) oled->drawPixel(pixelData->position, pixelData->colour);

		pixelData->position.x += dx;
		pixelData->position.y += dy;

		data <<= 1;
	} while (data != 0);
}

uint8_t Font::fontVrefCalcFont()
{
	return 0;
}

uint8_t Font::fontVrefCalcTop()
{
	return (uint8_t)ascent + 1;
}

uint8_t Font::fontVrefCalcBottom()
{
	return (uint8_t)descent;
}

uint8_t Font::getFontPos()
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

#pragma endregion

void Font::updateRefHeights(void)
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

int8_t Font::drawGlyph(SSD1351 *oled, Point position, char c, Colour colour)
{
	uint8_t *data;
	int16_t w, h;
	GlyphData gData;
	int16_t ix, iy;
	glyph_t g = getGlyph(c, &gData);

	position.y += getFontPos();

	if (g == NULL) return 0;

	data = getGlyphDataStart(g);

	w = gData.width;
	h = gData.height;
	position.x += gData.x;
	position.y -= gData.y;

	w += 7;
	w /= 8;
	iy = position.y;
	iy -= h;
	iy++;

	for (int j = 0; j < h; j++)
	{
		ix = position.x;

		for (int i = 0; i < w; i++)
		{
			PixelData pData;
			pData.position.x = ix;
			pData.position.y = iy;
			pData.colour = colour;
			pData.data = pgm_read_byte(data);
			pData.direction = 0;

			draw8Pixel(oled, &pData);

			data++;
			ix += 8;
		}

		iy++;
	}

	return gData.dx;
}

void Font::setFontPosBaseline()
{
	fontVref = FontPositionBaseline;
}