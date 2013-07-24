#include "ssd1351.h"
#include "font.c"

static uint8_t initSeq[] =
{
	ESC_CS(0),    /* disable chip */
	ESC_DELAY(50),
	ESC_ADR(DC_COMMAND), /* instruction mode */
	ESC_RESET(20),   /* DO reset low pulse with 20 milliseconds */
	ESC_CS(1),    /* enable chip */
	ESC_DELAY(50),
	0xfd,    /* Command Lock */
	ESC_ADR(DC_DATA),
	0x12,
	ESC_ADR(DC_COMMAND), /* instruction mode */
	0xfd,
	ESC_ADR(DC_DATA),
	0xb1,    /* Command Lock */
	ESC_ADR(DC_COMMAND), /* instruction mode */
	0xae,    /* Set Display Off */
	ESC_ADR(DC_COMMAND), /* instruction mode */
	0xb3,
	ESC_ADR(DC_DATA),
	0xf1,    /* Front Clock Div */
	ESC_ADR(DC_COMMAND), /* instruction mode */
	0xca,
	ESC_ADR(DC_DATA),
	0x7f,    /* Set Multiplex Ratio */
	ESC_ADR(DC_COMMAND), /* instruction mode */
	0xa0,
	ESC_ADR(DC_DATA),
	0xb4,    /* Set Colour Depth */
	ESC_ADR(DC_COMMAND), /* instruction mode */
	0x15,
	ESC_ADR(DC_DATA),
	0x00, 0x7f,    /* Set Column Address */
	ESC_ADR(DC_COMMAND), /* instruction mode */
	0x75,
	ESC_ADR(DC_DATA),
	0x00, 0x7f,    /* Set Row Address */
	ESC_ADR(DC_COMMAND), /* instruction mode */
	0xa1,
	ESC_ADR(DC_DATA),
	0x00,    /* Set Display Start Line */
	ESC_ADR(DC_COMMAND), /* instruction mode */
	0xa2,
	ESC_ADR(DC_DATA),
	0x00,    /* Set Display Offset */
	ESC_ADR(DC_COMMAND), /* instruction mode */
	0xb5,
	ESC_ADR(DC_DATA),
	0x00,    /* Set GPIO */
	ESC_ADR(DC_COMMAND), /* instruction mode */
	0xab,
	ESC_ADR(DC_DATA),
	0x01,    /* Set Function Selection */
	ESC_ADR(DC_COMMAND), /* instruction mode */
	0xb1,
	ESC_ADR(DC_DATA),
	0x32,    /* Set Phase Length */
	ESC_ADR(DC_COMMAND), /* instruction mode */
	0xb4,
	ESC_ADR(DC_DATA),
	0xa0, 0xb5, 0x55,  /* Set Segment Low Voltage */
	ESC_ADR(DC_COMMAND), /* instruction mode */
	0xbb,
	ESC_ADR(DC_DATA),
	0x17,    /* Set Precharge Voltage */
	ESC_ADR(DC_COMMAND), /* instruction mode */
	0xbe,
	ESC_ADR(DC_DATA),
	0x05,    /* Set VComH Voltage */
	ESC_ADR(DC_COMMAND), /* instruction mode */
	0xc1,
	ESC_ADR(DC_DATA),
	0xc8, 0x80, 0xc8,  /* Set Contrast */
	ESC_ADR(DC_COMMAND), /* instruction mode */
	0xc7,
	ESC_ADR(DC_DATA),
	0x0f,    /* Set Master Contrast */
	ESC_ADR(DC_COMMAND), /* instruction mode */
	0xb6,
	ESC_ADR(DC_DATA),
	0x01,    /* Set Second Precharge Period */
	ESC_ADR(DC_COMMAND), /* instruction mode */
	0xa6,    /* Set Display Mode Reset */
	ESC_ADR(DC_COMMAND), /* instruction mode */
	0xb8,    /* Set CMD Grayscale Lookup */
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
	0xaf,     /* Set Display On */
	0x5c,
	ESC_DELAY(50),
	ESC_CS(0),    /* disable chip */
	ESC_ADR(DC_DATA),
	ESC_END     /* end of sequence */
};

int ssd1351_init(uint8_t chan, uint8_t dcPin, uint8_t resetPin, uint8_t csPin)
{
	displayCom_setupPins(chan, dcPin, resetPin, csPin);
	
	if (displayCom_writeEscSeq(initSeq) == - 1)
	{
		return - 1;
	}
}

void ssd1351_setFont(rom uint8_t *f)
{
	if (fontData != f)
	{
		fontData = f;

		if (f != NULL)
		{
			updateRefHeights();
			setFontPosBaseline();
		}
	}
}

signed int16 sgn(signed int16 val)
{
   if (val < 0)
      return -1;
   else
      return 1;
}

void ssd1351_setCursor(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
	if (x >= 127) x = 127;
	if (y >= 127) y = 127;
	if (w >= 127) w = 127;
	if (h >= 127) h = 127;
	
	uint8_t cursorSeq[] = {
	   ESC_CS(1),
	   ESC_ADR(DC_COMMAND),
	   0x15,
	   ESC_ADR(DC_DATA),
	   0, 0,
	   
	   ESC_ADR(DC_COMMAND),
	   0x75,
	   ESC_ADR(DC_DATA),
	   0, 0,
	   
	   ESC_ADR(DC_COMMAND),
	   0x5c,
	   
	   ESC_ADR(DC_DATA),
	   ESC_CS(0),
	   ESC_END
	  };

	cursorSeq[10] = x;
	cursorSeq[11] = w;
	cursorSeq[19] = y;
	cursorSeq[20] = h;
	
	displayCom_writeEscSeq(cursorSeq);
}

void ssd1351_fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, COLOUR colour)
{
	uint8_t dX, dY;
	
	ssd1351_setCursor(x, y, x + w - 1, y + h - 1);
	displayCom_enableChip(true);
	
	for (dY = y; dY < h; dY++) 
	{
		for (dX = x; dX < w; dX++)
		{
			displayCom_writeData(colour.r >> 2);
			displayCom_writeData(colour.g >> 2);
			displayCom_writeData(colour.b >> 2);
		}	
	}	
	
	displayCom_enableChip(false);
}

void ssd1351_drawPixel(uint8_t x, uint8_t y, COLOUR colour)
{
	if (x > 128 || y > 128) return;
   
	ssd1351_setCursor(x, y, x, y);
	
	displayCom_enableChip(true);
	
	displayCom_writeData(colour.r >> 2);
	displayCom_writeData(colour.g >> 2);
	displayCom_writeData(colour.b >> 2);
	
	displayCom_enableChip(false);
}

void ssd1351_drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, COLOUR colour)
{
	uint8_t dx, dy;
	int8_t dyStep;
	bool steep;
	int16_t calc;

	// Vertical Line
	if (x1 == x2)
	{
		if (y1 > y2)
		{
			ssd1351_swaps(&y1, &y2);
		}

		ssd1351_fillRect(x1, y1, 1, y2 - y1, colour);
		return;
	}

	// Horizontal Line
	if (y1 == y2)
	{
		if (x1 > x2)
		{
			ssd1351_swaps(&x1, &x2);
		}

		ssd1351_fillRect(x1, y1, x2 - x1, 1, colour);
		return;
	}

	steep = abs(y2 - y1) > abs(x2 - x1);

	if (steep)
	{
		ssd1351_swaps(&x1, &y1);
		ssd1351_swaps(&x2, &y2);
	}

	if (x1 > x2)
	{
		ssd1351_swaps(&x1, &x2);
		ssd1351_swaps(&y1, &y2);
	}

	dx = x2 - x1;
	dy = abs(y2 - y1);
	calc = dx >> 1;
	dyStep = (y1 < y2 ? 1 : -1);

	for (; x1 <= x2; x1++)
	{
		if (steep)
		{
			ssd1351_drawPixel(y1, x1, colour);
		}
		else
		{
			ssd1351_drawPixel(x1, y1, colour);
		}

		calc -= dy;

		if (calc < 0)
		{
			y1 += dyStep;
			calc += dx;
		}
	}
}

void ssd1351_drawString(POINT position, char *str, COLOUR colour)
{
	int8_t d;
	uint8_t x = position.x;
	uint8_t y = position.y;
	
	while (*str != '\0')
	{
		d = drawGlyph(position, *str, colour);
		position.x += d;
		str++;
	}
}

void ssd1351_rotateScreen()
{
	displayCom_enableChip(true);

	displayCom_setData(false);
	displayCom_writeData(0xa0);

	displayCom_setData(true);
	displayCom_writeData(0xa6);

	displayCom_enableChip(false);
}
	
void ssd1351_unRotateScreen()
{
	displayCom_enableChip(true);

	displayCom_setData(false);
	displayCom_writeData(0xa0);

	displayCom_setData(true);
	displayCom_writeData(0xb4);
	
	displayCom_enableChip(false);
}	

void ssd1351_swaps(uint8_t *x, uint8_t *y)
{
	uint8_t t = *x;

	*x = *y;
	*y = t;
}