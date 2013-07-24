/*
 * File:   WiringPic.h
 * Author: Jamjar
 *
 * Created on 24 June 2013, 10:56
 */

#ifndef WiringPic_H
#define WiringPic_H

#define   INPUT	    0
#define   OUTPUT    1

#define   LOW	    0
#define   HIGH	    1

void pinMode(int pin, int mode);
void digitalWrite(int pin, int value);
void delay(int msdelay);
int wiringPicSPIDataRW(int chan, byte* data, int len);

#endif
