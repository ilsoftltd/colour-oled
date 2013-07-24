#ifndef font_H
#define font_H

#include <stdint.h>
#include <stdbool.h>
#include "drawing.c"
#include "ssd1351.h"

#define FONT_DATA_STRUCT_SIZE 17
#define NULL 0

const uint8_t HeightModeText = 0;
const uint8_t HeightModeXText = 1;
const uint8_t HeightModeAll = 2;

typedef rom uint8_t *glyph_t;

typedef struct GlyphData
{
	int8_t x;
	int8_t y;
	uint8_t gWidth;
	uint8_t gHeight;
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
	FontPositionBottom = 1,
	FontPositionBaseline = 2
};

rom uint8_t *fontData = 0;
uint8_t fontHeightMode = 0;
uint8_t lineSpacing = 0;
uint8_t lineSpacingFactor = 64;
int8_t ascent = 0;
int8_t descent = 0;
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
glyph_t getGlyph(char c, GLYPH_DATA *glyphData);
GLYPH_DATA getGlyphData(rom uint8_t *g);
struct GlyphData getEmptyGlyphData();
uint8_t *getGlyphDataStart(glyph_t g);
void draw8Pixel(PPIXEL_DATA *pixelData);
uint8_t fontVrefCalcFont();
uint8_t fontVrefCalcTop();
uint8_t fontVrefCalcBottom();
uint8_t getFontPos();
void updateRefHeights(void);
int8_t drawGlyph(POINT position, char c, COLOUR colour);

#endif