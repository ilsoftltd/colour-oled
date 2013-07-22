/*

  Fontb.h
  
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

#ifndef _FONTb_h
#define _FONTb_h

#include <stdint.h>

#include "defs.h"
#include "Drawing.h"
#include "SSD1351b.h"

#define FONT_DATA_STRUCT_SIZE 17

class SSD1351b;

enum FontHeightMode
{
	HeightModeText = 0,
	HeightModeXText,
	HeightModeAll
};

typedef uint8_t *glyph_t;

typedef struct GlyphData
{
	int8_t x;
	int8_t y;
	uint8_t width;
	uint8_t height;
	uint8_t dx;
} GLYPH_DATA;

typedef struct PixelData
{
	Point position;
	uint8_t direction;
	uint8_t data;
	Colour colour;
} PIXEL_DATA;

enum FontPosition
{
	FontPositionTop = 0,
	FontPositionBottom,
	FontPositionBaseline
};

class Fontb
{
private:
	FontHeightMode fontHeightMode;
	uint8_t *fontData;
	uint8_t lineSpacing;
	uint8_t lineSpacingFactor;
	int8_t ascent;
	int8_t descent;
	FontPosition fontVref;

	uint8_t getFontByte(uint8_t offset);
	uint16_t getFontWord(uint8_t offset);
	uint8_t getFontFormat(void);
	uint8_t getBBXWidth(void);
	uint8_t getBBXHeight(void);
	uint8_t getBBXOffX(void);
	uint8_t getBBXOffY(void);
	uint8_t getCapitalAHeight(void);
	uint16_t getEncoding65Pos(void);
	uint16_t getEncoding97Pos(void);
	uint8_t getFontStartEncoding(void);
	uint8_t getFontEndEncoding(void);
	uint8_t getLowerGDesent(void);
	uint8_t getXAscent(void);
	uint8_t getXDescent(void);
	uint8_t getAscent(void);
	uint8_t getDescent(void);
	uint8_t getGlyphStructureSize();
	glyph_t getGlyph(char c, GlyphData *glyphData);
	GlyphData getGlyphData(glyph_t g);
	GlyphData getEmptyGlyphData();
	uint8_t *getGlyphDataStart(glyph_t g);
	void draw8Pixel(SSD1351b *oled, PixelData *pixelData);
	uint8_t fontVrefCalcFont();
	uint8_t fontVrefCalcTop();
	uint8_t fontVrefCalcBottom();
	uint8_t getFontPos();

public:
	Fontb(uint8_t *fontData);
	~Fontb(void);

	void updateRefHeights(void);
	int8_t drawGlyph(SSD1351b *oled, Point position, char c, Colour colour);
	
	void setFontPosBaseline();
};

#endif
