
#ifndef WS2812_H
#define WS2812_H

#include <stdint.h>

typedef unsigned int	uint32_t;

typedef union {
  struct  {
    uint8_t r, g, b;
  };
  uint32_t num;
} rgbVal;
/*
typedef union {
  struct __attribute__ ((packed)) {
    uint8_t r, g, b;
  };
  uint32_t num;
} rgbVal1;
*/

extern void WS2812_Init(void);
extern void WS2812_SetColors(unsigned int length, rgbVal *array);

inline rgbVal makeRGBVal(uint8_t r, uint8_t g, uint8_t b)
{
  rgbVal v;
  v.r = r;
  v.g = g;
  v.b = b;
  return v;
}

#endif