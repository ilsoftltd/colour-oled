/*
 * File:   displayCom.h
 * Author: Jamjar
 *
 * Created on 24 June 2013, 11:02
 */

#ifndef displayCom_H
#define displayCom_H

#include <stdint.h>
#include <stdbool.h>

#include "wiringPic.c"

#define DC_COMMAND  	0
#define DC_DATA     	1
	
#define CHANNEL     	0
#define CS_PIN      	1
#define DC_PIN      	2
#define RESET_PIN   	3
	
#define DC_DELAY    	0x01
#define DC_CS       	0x02
#define DC_ADR      	0x03
#define DC_RESET    	0x04
#define DC_END      	0xfe

#define ESC_DELAY(x)    0xff, DC_DELAY, x
#define ESC_CS(x)       0xff, DC_CS, x
#define ESC_ADR(x)      0xff, DC_ADR, x
#define ESC_RESET(x)    0xff, DC_RESET, x
#define ESC_END         0xff, DC_END

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

int displayCom_setupPins(uint8_t chan, uint8_t dcPin, uint8_t resetPin, uint8_t csPin);
int displayCom_writeEscSeq(uint8_t *escSeq);
int displayCom_writeData(uint8_t data);
int displayCom_writeDataArray(uint8_t *data, uint16_t len);
void displayCom_enableChip(bool enable);
void displayCom_setData(bool isData);
void displayCom_reset(uint8_t delayValue);

#endif
