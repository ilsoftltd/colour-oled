/*
 * File:   ssd1351.h
 * Author: Jamjar
 *
 * Created on 24 June 2013, 11:12
 */

#ifndef ssd1351_H
#define ssd1351_H

#include <stdint.h>
#include <stdbool.h>

#include "displayCom.c"
#include "font.h"
#include "drawing.c"

#define WIDTH   128
#define HEIGHT  128

bool intersects(uint8_t y, uint8_t h);
int ssd1351_init(uint8_t chan, uint8_t dcPin, uint8_t resetPin, uint8_t csPin);
void ssd1351_setFont(rom uint8_t *f);
void ssd1351_setCursor(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void ssd1351_fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, COLOUR colour);
void ssd1351_drawPixel(uint8_t x, uint8_t y, COLOUR colour);
void ssd1351_drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h, COLOUR colour);
void ssd1351_drawString(POINT position, char *str, COLOUR colour);
void ssd1351_rotateScreen();
void ssd1351_unRotateScreen();
void ssd1351_setFont(uint8_t *font);
void ssd1351_swaps(uint8_t *x, uint8_t *y);

#endif
