// DisplayCom.h

#ifndef _DISPLAYCOM_h
#define _DISPLAYCOM_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "utility/defs.h"
#include "utility/Drawing.h"

#define DC_COMMAND	0
#define DC_DATA		1

#define DC_DELAY	0x01
#define DC_CS		0x02
#define DC_ADR		0x03
#define DC_RESET	0x04
#define DC_END		0xfe

#define ESC_DELAY(x)	0xff, DC_DELAY, x
#define ESC_CS(x)		0xff, DC_CS, x
#define ESC_ADR(x)		0xff, DC_ADR, x
#define ESC_RESET(x)	0xff, DC_RESET, x
#define ESC_END			0xff, DC_END

#define CS_PIN		0
#define DC_PIN		1
#define RESET_PIN	2

class DisplayCom
{
private:
	uint8_t pins[3];

public:
	void setupPins(uint8_t csPin, uint8_t dcPin, uint8_t resetPin);
	uint8_t writeEscSeq(const uint8_t *escSeq);
	uint8_t writeData(const uint8_t data);
	uint8_t writeDataArray(const uint8_t *data, size_t size);
	uint8_t writeColour(const Colour colour);
	uint8_t writeColourBuf(const Colour *colour, size_t size);
	void enableChip(bool enable);
	void setData(bool isData);
	void reset(uint8_t delayValue);
};

#endif

