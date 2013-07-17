// Font.h

#ifndef _FONTb_h
#define _FONTb_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "utility/defs.h"
#include "utility/Drawing.h"
#include "utility/SSD1351b.h"

#define FONT_DATA_STRUCT_SIZE 17

class SSD1351b;

enum FontHeightMode
{
	HeightModeText = 0,
	HeightModeXText,
	HeightModeAll
};

typedef prog_uint8_t *glyph_t;

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
	prog_uint8_t *fontData;
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
	Fontb(prog_uint8_t *fontData);
	~Fontb(void);

	void updateRefHeights(void);
	int8_t drawGlyph(SSD1351b *oled, Point position, char c, Colour colour);
	
	void setFontPosBaseline();
};

#endif
