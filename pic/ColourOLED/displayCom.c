#include "displayCom.h"

uint8_t pins[4];

int displayCom_setupPins(uint8_t chan, uint8_t dcPin, uint8_t resetPin, uint8_t csPin)
{
    if (chan > 1) chan = 1;

    pins[CHANNEL] = chan;
    pins[CS_PIN] = csPin;
    pins[DC_PIN] = dcPin;
    pins[RESET_PIN] = resetPin;

    return 0;
}

int displayCom_writeEscSeq(uint8_t *escSeq)
{
	bool isEsc = false;
	uint8_t cmd;
	uint8_t value;
	uint16_t i = 0;

	// Loop until we find the ESC_END command.
	for (;;)
	{
		value = *escSeq;

		if (!isEsc)
		{
			if (value != 0xff)
			{
				displayCom_writeData(value);
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

			if (cmd == DC_END) return 0;

			value = *escSeq;

			switch (cmd)
			{
			case DC_DELAY:
				delay(value);
				break;

			case DC_CS:
				displayCom_enableChip(value);
				break;

			case DC_ADR:
				displayCom_setData(value);
				break;

			case DC_RESET:
				displayCom_reset(value);
				break;
			}
	
			isEsc = false;
		}
	
		escSeq++;
		i++;
	
		if (i > 1000)
		{
			// Loop broke, to stop the hang, return out with a -1.
			return -1;
		}
	}		
}

int displayCom_writeData(uint8_t data)
{
	uint8_t b[1];
	b[0] = data;

	displayCom_writeDataArray(b, 1);

	return 0;
}

int displayCom_writeDataArray(uint8_t *data, uint16_t len)
{
	if (wiringPicSPIDataRW(pins[CHANNEL], data, len) == -1)
	{
		return -1;
	}

	return 0;
}

void displayCom_enableChip(bool enable)
{
	digitalWrite(pins[CS_PIN], !enable);
	delay(1);
}

void displayCom_setData(bool isData)
{
	digitalWrite(pins[DC_PIN], isData);
	delay(1);
}

void displayCom_reset(uint8_t delayValue)
{
	digitalWrite(pins[RESET_PIN], LOW);
	delay(delayValue);
	digitalWrite(pins[RESET_PIN], HIGH);
	delay(2);
}