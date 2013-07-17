// SSD1351.h

#ifndef _SSD1351b_h
#define _SSD1351b_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "utility/defs.h"
#include "utility/Drawing.h"
#include "utility/DisplayCom.h"
#include "utility/Fontb.h"
#include "utility/Bitmap.h"

#define WIDTH	128
#define HEIGHT	128

#define SSD1351_MODE	SSD1351_MODE_BUFFER

#pragma region InitSeq

static const prog_uint8_t initSeq[] PROGMEM = {
	ESC_CS(0),					/* disable chip */
	ESC_DELAY(50),
	ESC_ADR(DC_COMMAND),		/* instruction mode */
	ESC_RESET(20),				/* do reset low pulse with 20 milliseconds */
	ESC_CS(1),					/* enable chip */
	ESC_DELAY(50),

	0xfd,						/* Command Lock */
	ESC_ADR(DC_DATA),
	0x12,						

	ESC_ADR(DC_COMMAND),		/* instruction mode */
	0xfd,
	ESC_ADR(DC_DATA),
	0xb1,						/* Command Lock */

	ESC_ADR(DC_COMMAND),		/* instruction mode */
	0xae,						/* Set Display Off */

	ESC_ADR(DC_COMMAND),		/* instruction mode */
	0xb3,
	ESC_ADR(DC_DATA),
	0xf1,						/* Front Clock Div */

	ESC_ADR(DC_COMMAND),		/* instruction mode */
	0xca,
	ESC_ADR(DC_DATA),
	0x7f,						/* Set Multiplex Ratio */

	ESC_ADR(DC_COMMAND),		/* instruction mode */
	0xa0,
	ESC_ADR(DC_DATA),
	0xb4,						/* Set Colour Depth */

	ESC_ADR(DC_COMMAND),		/* instruction mode */
	0x15,
	ESC_ADR(DC_DATA),
	0x00, 0x7f,					/* Set Column Address */

	ESC_ADR(DC_COMMAND),		/* instruction mode */
	0x75,
	ESC_ADR(DC_DATA),
	0x00, 0x7f,					/* Set Row Address */

	ESC_ADR(DC_COMMAND),		/* instruction mode */
	0xa1,
	ESC_ADR(DC_DATA),
	0x00,						/* Set Display Start Line */

	ESC_ADR(DC_COMMAND),		/* instruction mode */
	0xa2,
	ESC_ADR(DC_DATA),
	0x00,						/* Set Display Offset */

	ESC_ADR(DC_COMMAND),		/* instruction mode */
	0xb5,
	ESC_ADR(DC_DATA),
	0x00,						/* Set GPIO */

	ESC_ADR(DC_COMMAND),		/* instruction mode */
	0xab,
	ESC_ADR(DC_DATA),
	0x01,						/* Set Function Selection */

	ESC_ADR(DC_COMMAND),		/* instruction mode */
	0xb1,
	ESC_ADR(DC_DATA),
	0x32,						/* Set Phase Length */

	ESC_ADR(DC_COMMAND),		/* instruction mode */
	0xb4,
	ESC_ADR(DC_DATA),
	0xa0, 0xb5, 0x55,			/* Set Segment Low Voltage */

	ESC_ADR(DC_COMMAND),		/* instruction mode */
	0xbb,
	ESC_ADR(DC_DATA),
	0x17,						/* Set Precharge Voltage */

	ESC_ADR(DC_COMMAND),		/* instruction mode */
	0xbe,
	ESC_ADR(DC_DATA),
	0x05,						/* Set VComH Voltage */

	ESC_ADR(DC_COMMAND),		/* instruction mode */
	0xc1,
	ESC_ADR(DC_DATA),
	0xc8, 0x80, 0xc8,			/* Set Contrast */

	ESC_ADR(DC_COMMAND),		/* instruction mode */
	0xc7,
	ESC_ADR(DC_DATA),
	0x0f,						/* Set Master Contrast */

	ESC_ADR(DC_COMMAND),		/* instruction mode */
	0xb6,
	ESC_ADR(DC_DATA),
	0x01,						/* Set Second Precharge Period */

	ESC_ADR(DC_COMMAND),		/* instruction mode */
	0xa6,						/* Set Display Mode Reset */


	ESC_ADR(DC_COMMAND),		/* instruction mode */
	0xb8,						/* Set CMD Grayscale Lookup */
	ESC_ADR(DC_DATA),
	0x05,
	0x06,
	0x07,
	0x08,
	0x09,
	0x0a,
	0x0b,
	0x0c,
	0x0D,
	0x0E,
	0x0F,
	0x10,
	0x11,
	0x12,
	0x13,
	0x14,
	0x15,
	0x16,
	0x18,
	0x1a,
	0x1b,
	0x1C,
	0x1D,
	0x1F,
	0x21,
	0x23,
	0x25,
	0x27,
	0x2A,
	0x2D,
	0x30,
	0x33,
	0x36,
	0x39,
	0x3C,
	0x3F,
	0x42,
	0x45,
	0x48,
	0x4C,
	0x50,
	0x54,
	0x58,
	0x5C,
	0x60,
	0x64,
	0x68,
	0x6C,
	0x70,
	0x74,
	0x78,
	0x7D,
	0x82,
	0x87,
	0x8C,
	0x91,
	0x96,
	0x9B,
	0xA0,
	0xA5,
	0xAA,
	0xAF,
	0xB4,

	ESC_ADR(DC_COMMAND),
	0xaf,							/* Set Display On */
	0x5c,
	ESC_DELAY(50),
	ESC_CS(0),						/* disable chip */
	ESC_ADR(DC_DATA),
	ESC_END							/* end of sequence */
};

#pragma endregion

#pragma region ColRowSeq

static const uint8_t colRowSeq[] PROGMEM = {
	ESC_CS(1),
	ESC_ADR(DC_COMMAND),	0x15,
	ESC_ADR(DC_DATA),		0x00, 0x7f,
	ESC_ADR(DC_COMMAND),	0x75,
	ESC_ADR(DC_DATA),		0x00, 0x7f,
	ESC_ADR(DC_COMMAND),	0x5c,
	ESC_ADR(DC_DATA),
	ESC_CS(0),
	ESC_END
};

#pragma endregion

enum Orientation
{
	CW0 = 0,
	CW90,
	CW180,
	CW270,
};

class Fontb;

class SSD1351b
{
private:

	bool isInitialised;
	Fontb *font;
	Colour *lineBuf;
	int16_t currentLine;

	inline void swap(int16_t *x, int16_t *y);
	inline bool intersects(int16_t y, int16_t h);

public:
	DisplayCom *com;
	SSD1351b(uint8_t csPin, uint8_t dcPin, uint8_t resetPin);
	~SSD1351b();

	void firstLine();
	bool nextLine();
	void setFont(Fontb *font);
	void clearScreen(Colour colour);
	void drawPixel(Point pixel, Colour colour);
	void drawLine(Point start, Point end, Colour colour);
	void drawRect(Rectangle rect, Colour colour);
	void fillRect(Rectangle rect, Colour colour);
	void drawCircle(Circle circle, Colour colour);
	void fillCircle(Circle circle, Colour colour);
	void drawPolygon(Point *points, int length, Colour colour);
	void drawString(Point position, char* str, Colour colour);
	void drawBitmap(Point position, Bitmap *bitmap);
	void setOrientation(Orientation orientation);
	void setInvert(bool inverted);
	void screenOn();
	void screenOff();
	void setContrast(uint8_t contrast);
};

#endif
