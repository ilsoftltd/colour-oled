#include "utility/DisplayCom.h"

void DisplayCom::setupPins(uint8_t csPin, uint8_t dcPin, uint8_t resetPin)
{
	pins[CS_PIN] = csPin;
	pins[DC_PIN] = dcPin;
	pins[RESET_PIN] = resetPin;

	pinMode(csPin, OUTPUT);
	pinMode(dcPin, OUTPUT);
	pinMode(resetPin, OUTPUT);
	pinMode(SS, OUTPUT);
	pinMode(SCK, OUTPUT);
	pinMode(MOSI, OUTPUT);

	digitalWrite(csPin, HIGH);
	digitalWrite(dcPin, HIGH);
	digitalWrite(resetPin, HIGH);
	digitalWrite(SS, HIGH);

	SPCR = 0;
	SPCR = (1 << SPE) | (1 << MSTR) | (0 << SPR1) | (0 << SPR0) | (0 << CPOL) | (0 << CPHA);
	SPSR |= (1 << SPI2X);
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
				writeData(value);
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

uint8_t DisplayCom::writeData(const uint8_t data)
{
	// Set the data and send.
	SPDR = data;

	// Wait until it's sent all the data.
	while (!(SPSR & (1 << SPIF)));

	// Clear the SPIF flag by reading SPDR.
	return SPDR;
}

uint8_t DisplayCom::writeDataArray(const uint8_t *data, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		writeData(data[i]);
	}

	return 0;
}

uint8_t DisplayCom::writeColour(const Colour colour)
{
	writeData(colour.r);
	writeData(colour.g);
	writeData(colour.b);

	return 0;
}

uint8_t DisplayCom::writeColourBuf(const Colour *colour, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		writeColour(colour[i]);
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