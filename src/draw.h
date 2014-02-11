#ifndef __INCLUDED_DRAW_H__
#define __INCLUDED_DRAW_H__

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "constants.h"
#include "framebuffer.h"
#include "game_types.h"
#include "game.h"
#include "util.h"

#ifdef __cplusplus
extern "C" {
#endif


void drawLine(int x0, int y0, int x1, int y1, int val);
void drawBar(bar_t *bar, bool use_stipple);
void drawBars(bool use_stipple);
void drawExplodingBars(void);
void drawFill(void);

void drawPlayer(player_t *p);
void drawCentergon(uint8_t outer_radius, uint8_t order, bool with_lines);
void drawCentergonArms(void);

void drawBezier(
    int x0, int y0,
    int vx0, int vy0,
    int x1, int y1,
    int vx1, int vy1,
    int val);

void calculateRotation(int *x, int *y);

extern const uint8_t Pixel_dark;
extern const uint8_t Pixel_bright;

#ifdef __cplusplus
}
#endif


#endif

