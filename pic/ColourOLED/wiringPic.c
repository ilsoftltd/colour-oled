#include "wiringPic.h"

void pinMode(int pin, int mode)
{
    //Not implemented - pin mode set in include file
}

void digitalWrite(int pin,int value)
{
    if(pin == 0) return;

    output_bit(pin, value);
}

void delay(int msdelay)
{
    delay_ms(msdelay);
}

int wiringPicSPIDataRW(int chan, byte* data, uint16_t len)
{
    uint16_t a;

    if(chan == 0)
    {
		for(a = 0; a < len; a++)
		{
	    	spi_write(data[a]);
		}
    }
    else
    {
		for(a = 0; a < len; a++)
		{
	    	spi_write2(data[a]);
		}
    }

    return 0;
}
