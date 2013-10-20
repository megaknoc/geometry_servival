#ifndef __INCLUDED_DRAW_H__
#define __INCLUDED_DRAW_H__

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "constants.h"
#include "framebuffer.h"
#include "game_types.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

void drawLine(int x0, int y0, int x1, int y1, int val);
void drawBar(bar_t *bar);
void drawBars(void);
void drawExplodingBars(void);

void drawPlayer(uint8_t x, uint8_t y);
void drawCentergon(uint8_t outer_radius, uint8_t order);

extern const uint8_t Pixel_dark;
extern const uint8_t Pixel_bright;

#endif

