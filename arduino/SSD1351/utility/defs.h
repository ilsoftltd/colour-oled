// defs.h

#ifndef _SSD1351_DEFS_h
#define _SSD1351_DEFS_h

#if defined(__AVR__)
#include <avr/pgmspace.h>
#endif 

#ifdef __18CXX
typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
#else
#include <stdint.h>
#endif

#define SSD1351_MODE_DIRECT	0
#define SSD1351_MODE_BUFFER	1

#endif

