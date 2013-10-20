#ifndef __INCLUDED_FRAMEBUFFER_H__
#define __INCLUDED_FRAMEBUFFER_H__

#include <stdint.h>
#include <assert.h>
#include "constants.h"

#ifdef __cplusplus
extern "C" {
#endif

extern struct framebuffer_t {
    uint8_t width;
    uint8_t height;
    uint8_t data[FRAMEBUFFER_WIDTH*FRAMEBUFFER_HEIGHT]; // one byte per pixel :}
} framebuffer;

uint8_t *framebufferPixel(uint8_t x, uint8_t y);
void framebufferSet(uint8_t x, uint8_t y, uint8_t val);
void framebufferClear(uint8_t val);

#ifdef __cplusplus
}
#endif

#endif

