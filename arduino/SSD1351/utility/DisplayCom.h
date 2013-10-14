/*

  DisplayCom.h
  
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

#ifndef _DISPLAYCOM_h
#define _DISPLAYCOM_h

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

#include "utility/defs.h"
#include "utility/Drawing.h"
#include "pins_arduino.h"

class DisplayCom
{
private:
	uint8_t pins[3];

public:
	void setupPins(uint8_t csPin, uint8_t dcPin, uint8_t resetPin);
	uint8_t writeEscSeq(const uint8_t *escSeq);
#ifdef __AVR__
	uint8_t writeData(const uint8_t data);
#else
	uint8_t writeData(const uint8_t data, bool cont);
#endif
	uint8_t writeDataArray(const uint8_t *data, size_t size);

#ifdef __AVR__
	uint8_t writeColour(const Colour colour);
#else
	uint8_t writeColour(const Colour colour, bool cont);
#endif

	uint8_t writeColourBuf(const Colour *colour, size_t size);
	void enableChip(bool enable);
	void setData(bool isData);
	void reset(uint8_t delayValue);
};

#endif

