#include "draw.h"

extern bool generateCorners(int8_t *buf, uint8_t radius, uint8_t num_corners);

const uint8_t Pixel_dark    = 0x00;
const uint8_t Pixel_bright  = 0xff;

/**
 * @brief Draw a line from (x,y) to (x2, y2).
 */
void drawLine(int x0, int y0, int x1, int y1, int val)
{
    int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = dx+dy, e2; /* error value e_xy */

    while(1) {
        framebufferSet(x0, y0, val);
        if (x0==x1 && y0==y1) break;
        e2 = 2*err;
        if (e2 > dy) { err += dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
    }
}

/**
 * @brief Draw a bar in its sector.
 * @detail The current shape determines the appeareance of the bar.
 */
void drawBar(bar_t *bar)
{
    assert(bar != NULL && bar->valid);
    assert(bar->sector < game.shape);

    int8_t cs[2*(game.shape+1)];
    generateCorners(cs, bar->dist, game.shape);

    const int i = bar->sector;
    drawLine(
        GAME_CENTER_X + cs[2*i+0], GAME_CENTER_Y + cs[2*i+1],
        GAME_CENTER_X + cs[2*i+2], GAME_CENTER_Y + cs[2*i+3],
        Pixel_bright);
}

/**
 * @brief Draw an exploding bar.
 */
void drawExplodingBar(bar_t *bar)
{
    assert(bar != NULL && bar->valid);
    assert(bar->sector < game.shape);
    assert(bar->exploding);

    int8_t cs[2*(game.shape+1)];
    generateCorners(cs, bar->dist, game.shape);

    const int i = bar->sector;
    // TODO
    // break the bar into smaller bars and throw them away in the direction
    // they are coming from

    // XXX random pixels as the explosion for now
    // The direction can be computed from the sector and the current game
    // shape.
    uint8_t x0, x1, y0, y1;
    x0 = GAME_CENTER_X + cs[2*i+0];
    y0 = GAME_CENTER_Y + cs[2*i+1];
    x1 = GAME_CENTER_X + cs[2*i+2];
    y1 = GAME_CENTER_Y + cs[2*i+3];

    if ((bar->timer % 5) < 3) {
        framebufferSet(x0+2, y0-1, Pixel_bright);
        framebufferSet(x1-1, y1+2, Pixel_bright);
    } else {
        framebufferSet(x0+3, y0+1, Pixel_bright);
        framebufferSet(x1-2, y1, Pixel_bright);
    }
}

void drawExplodingBars(void)
{
    int i;
    for (i=0; i<MAX_BARS; i++) {
        bar_t *b = &game.bars[i];
        if (b->valid && b->exploding) {
            drawExplodingBar(b);
        }
    }
}

void drawBars(void)
{
    int i;
    for (i=0; i<MAX_BARS; i++) {
        bar_t *b = &game.bars[i];
        if (b->valid) {
            drawBar(b);
        }
    }
}

/**
 * @brief Draw the player at coordinates (x, y) looking in direction.
 * @detail
 */
void drawPlayer(uint8_t x, uint8_t y)
{
    const uint8_t val = Pixel_bright;

    framebufferSet(x,   y,   val);
    framebufferSet(x-1, y,   val);
    framebufferSet(x,   y-1, val);
    framebufferSet(x+1, y,   val);
    framebufferSet(x,   y+1, val);
}

/**
 * @brief Draw the $gon at the center.
 * @detail The *gon changes with the current level $gon.
 */
void drawCentergon(uint8_t outer_radius, uint8_t order)
{
    int8_t corners[2*(order+1)];
    generateCorners(corners, outer_radius, order);

    int i;
    for (i=0; i<2*(order-1)+1; i += 2) {
        drawLine(
            GAME_CENTER_X + corners[i],   GAME_CENTER_Y + corners[i+1],
            GAME_CENTER_X + corners[i+2], GAME_CENTER_Y + corners[i+3],
            Pixel_bright);
    }
}

