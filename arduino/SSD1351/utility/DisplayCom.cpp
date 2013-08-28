/*

  DisplayCom.cpp
  
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

#include "utility/DisplayCom.h"

void DisplayCom::setupPins(uint8_t csPin, uint8_t dcPin, uint8_t resetPin)
{
	pins[CS_PIN] = csPin;
	pins[DC_PIN] = dcPin;
	pins[RESET_PIN] = resetPin;

	pinMode(csPin, OUTPUT);
	pinMode(dcPin, OUTPUT);
	pinMode(resetPin, OUTPUT);

#ifdef __AVR__
	pinMode(SS, OUTPUT);
	pinMode(SCK, OUTPUT);
	pinMode(MOSI, OUTPUT);
#endif

	digitalWrite(csPin, HIGH);
	digitalWrite(dcPin, HIGH);
	digitalWrite(resetPin, HIGH);

#ifdef __AVR__
	digitalWrite(SS, HIGH);
#endif

#ifdef __AVR__
	SPCR = 0;
	SPCR = (1 << SPE) | (1 << MSTR) | (0 << SPR1) | (0 << SPR0) | (0 << CPOL) | (0 << CPHA);
	SPSR |= (1 << SPI2X);
#else
	SPI.begin(csPin);
	SPI.setClockDivider(csPin, 6);
#endif	
}

uint8_t DisplayCom::writeEscSeq(const uint8_t *escSeq)
{
	bool isEsc = false;
	uint8_t cmd;
	uint8_t value;

	// Loop until we find the ESC_END command.
	for (;;)
	{
		value = pgm_read_byte(escSeq);

		if (!isEsc)
		{
			if (value != 0xff)
			{
#ifdef __AVR__
				writeData(value);
#else
				writeData(value, false);
#endif
			}
			else
			{
				isEsc = true;
			}
		}
		else
		{
			escSeq++;

			cmd = value;
			value = pgm_read_byte(escSeq);

			switch (cmd)
			{
			case DC_DELAY:
				delay(value);
				break;

			case DC_CS:
				enableChip(value);
				break;

			case DC_ADR:
				setData(value);
				break;

			case DC_RESET:
				reset(value);
				break;

			case DC_END:
				// This is the end of the read so return.
				return 0;
			}

			isEsc = false;
		}

		escSeq++;
	}
}

#ifdef __AVR__
uint8_t DisplayCom::writeData(const uint8_t data)
#else
uint8_t DisplayCom::writeData(const uint8_t data, bool cont)
#endif
{
#ifdef __AVR__
	// Set the data and send.
	SPDR = data;

	// Wait until it's sent all the data.
	while (!(SPSR & (1 << SPIF)));

	// Clear the SPIF flag by reading SPDR.
	return SPDR;
#else
	if (cont)
		SPI.transfer(pins[CS_PIN], data, SPI_CONTINUE);
	else
		SPI.transfer(pins[CS_PIN], data);
#endif	
}

uint8_t DisplayCom::writeDataArray(const uint8_t *data, size_t size)
{
	for (int i = 0; i < size; i++)
	{
#ifdef __AVR__
		writeData(data[i]);
#else
		writeData(data[i], i == size - 1);
#endif
	}

	return 0;
}

#ifdef __AVR__
uint8_t DisplayCom::writeColour(const Colour colour)
#else
uint8_t DisplayCom::writeColour(const Colour colour, bool cont)
#endif
{
#ifdef __AVR__
	writeData(colour.r >> 2);
	writeData(colour.g >> 2);
	writeData(colour.b >> 2);
#else
	writeData(colour.r >> 2, true);
	writeData(colour.g >> 2, true);
	writeData(colour.b >> 2, cont);
#endif

	return 0;
}

uint8_t DisplayCom::writeColourBuf(const Colour *colour, size_t size)
{
	for (int i = 0; i < size; i++)
	{
#ifdef __AVR__
		writeColour(colour[i]);
#else
		writeColour(colour[i], i == size - 1);
#endif
	}

	return 0;
}

void DisplayCom::enableChip(bool enable)
{
	digitalWrite(pins[CS_PIN], !enable);
}

void DisplayCom::setData(bool isData)
{
	digitalWrite(pins[DC_PIN], isData);
}

void DisplayCom::reset(uint8_t delayValue)
{
	digitalWrite(pins[RESET_PIN], LOW);
	delay(delayValue);
	digitalWrite(pins[RESET_PIN], HIGH);
	delay(2);
}