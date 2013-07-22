/*

  SSD1351d.h
  
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

#ifndef _SSD1351d_h
#define _SSD1351d_h

#include <stdint.h>
#include <stdlib.h>

#include "defs.h"
#include "Bitmap.h"
#include "Drawing.h"
#include "DisplayCom.h"
#include "Fontd.h"

#define WIDTH	128
#define HEIGHT	128

#define SSD1351_MODE	SSD1351_MODE_DIRECT

#pragma region InitSeq

static const uint8_t initSeq[] = {
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

enum Orientation
{
	CW0 = 0,
	CW180,
};

class Fontd;

class SSD1351d
{
private:

	bool isInitialised;
	Fontd *font;

	inline void drawPixelNoCS(Point pixel, Colour colour);
	inline void drawPixelNoSeq(Point pixel, Colour colour);
	inline void swap(int16_t *x, int16_t *y);
	void colRowSeq(Rectangle rect);

public:
	DisplayCom *com;
	SSD1351d(uint8_t channel, uint8_t csPin, uint8_t dcPin, uint8_t resetPin, uint32_t speed);
	~SSD1351d();

	void setFont(Fontd *font);
	void clearScreen(Colour colour);
	void drawPixel(uint16_t x, uint16_t y, Colour colour);
	void drawPixel(Point pixel, Colour colour);
	void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Colour colour);
	void drawLine(Point start, Point end, Colour colour);
	void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Colour colour);
	void drawRect(Rectangle rect, Colour colour);
	void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Colour colour);
	void fillRect(Rectangle rect, Colour colour);
	void drawCircle(uint16_t x, uint16_t y, uint16_t radius, Colour colour);
	void drawCircle(Circle circle, Colour colour);
	void fillCircle(uint16_t x, uint16_t y, uint16_t radius, Colour colour);
	void fillCircle(Circle circle, Colour colour);
	void drawPolygon(Point *points, int length, Colour colour);
	void drawString(uint16_t x, uint16_t y, char* str, Colour colour);
	void drawString(Point position, char* str, Colour colour);
	void drawBitmap(uint16_t x, uint16_t y, Bitmap *bitmap);
	void drawBitmap(Point position, Bitmap *bitmap);
	void setOrientation(Orientation orientation);
	void setInvert(bool inverted);
	void screenOn();
	void screenOff();
	void setContrast(uint8_t contrast);
};

#endif

