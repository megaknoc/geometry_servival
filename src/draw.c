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
 * @brief Draw a line from (x,y) to (x2, y2) with stipple pattern.
 * @detail The n'th bit in stipple will decide whether the n'th pixel of the
 * line will be drawn.
 */
void drawLineStipple(int x0, int y0, int x1, int y1, uint32_t stipple, int val)
{
    int dx =  abs(x1-x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1-y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

    int stipple_ix = 0;

    while(1) {
        if (stipple & _BV(stipple_ix)) {
            framebufferSet(x0, y0, val);
        }
        stipple_ix++;
        if (stipple_ix >= (sizeof(stipple) * 8)) {
            stipple_ix = 0;
        }
        if (x0 == x1 && y0 == y1) {
            break;
        }

        e2 = 2*err;

        if (e2 > dy) { err += dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
    }
}

uint32_t patternFromDist(uint8_t s)
{
    const uint32_t stipple_patterns[] = {
        0b11111111111111111111111111111111,
        0b11111111111101111111111111110111,
        0b01110111111101111111111111111111,
        0b11110111111001111111011110110110,
        0b10110101111100111111011101011111,
        0b11010111101101011011001101101111,
        0b10101111011001011101001110010011,
        0b00100111001001010101001010100010,
        0b00101100011000010111001001000010,
        0b00001100000100000110000000100000,
        0b00010000000000000100000000000010,
        0b00000000000000000000000000000000,
    };

    const int max = ARRAY_SIZE(stipple_patterns);
    int tmp = (s - (int) game.inner_radius - 2) / 4;
    if (tmp < 0) {
        tmp = 0;
    } else if (tmp >= max) {
        tmp = max-1;
    }
    return stipple_patterns[tmp];
}

/**
 * @brief Draw a bar in its sector.
 * @detail The current shape determines the appeareance of the bar.
 */
void drawBar(bar_t *bar, bool use_stipple)
{
    assert(bar != NULL && bar->valid);
   // assert(bar->sector < game.shape);
    assert(!bar->exploding);

    int8_t cs[2*(game.shape+1)];
    generateCorners(cs, bar->dist, game.shape);

    const int i = bar->sector;

    int x0 = cs[2*i+0];
    int y0 = cs[2*i+1];
    int x1 = cs[2*i+2];
    int y1 = cs[2*i+3];

    calculateRotation(&x0, &y0);
    calculateRotation(&x1, &y1);

    if (use_stipple) {
        drawLineStipple(
            GAME_CENTER_X + x0, GAME_CENTER_Y + y0,
            GAME_CENTER_X + x1, GAME_CENTER_Y + y1,
            bitrot_l(patternFromDist(bar->dist), bar->dist/2),
            Pixel_bright);
    } else {
        drawLine(
            GAME_CENTER_X + x0, GAME_CENTER_Y + y0,
            GAME_CENTER_X + x1, GAME_CENTER_Y + y1,
            Pixel_bright);
    }
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

/**
 * @brief Draw all exploding bars.
 */
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

/**
 * @brief Draw all living bars.
 */
void drawBars(bool use_stipple)
{
    int i;
    for (i=0; i<MAX_BARS; i++) {
        bar_t *b = &game.bars[i];
        if (b->valid && !b->exploding) {
            drawBar(b, use_stipple);
        }
    }
}

/**
 * @brief Draw the player at coordinates (x, y) looking in direction.
 * @detail
 */
void drawPlayer(player_t *p)
{
	assert(p != NULL && p->valid);
    const uint8_t val = Pixel_bright;

	uint8_t x = p->x;
	uint8_t y = p->y;

    framebufferSet(x,   y,   val);
    framebufferSet(x-1, y,   val);
    framebufferSet(x,   y-1, val);
    framebufferSet(x+1, y,   val);
    framebufferSet(x,   y+1, val);
}

/**
 * @brief Draw the $gon at the center.
 * @detail The *gon changes with the current level $gon.
 * @param[in] with_lines If true, draw lines between the corners of the hexagon.
 */
void drawCentergon(uint8_t outer_radius, uint8_t order, bool with_lines)
{
    int8_t corners[2*(order+1)];
    generateCorners(corners, outer_radius, order);

    int i;
    // TODO: whats's that number i<foo ?
    for (i=0; i<2*(order-1)+1; i += 2) {

        int x0 = corners[i];
        int y0 = corners[i+1];
        int x1 = corners[i+2];
        int y1 = corners[i+3];

        calculateRotation(&x0, &y0);
        calculateRotation(&x1, &y1);

        const int final_coords[] = {
            GAME_CENTER_X + x0,     GAME_CENTER_Y + y0,
            GAME_CENTER_X + x1,   GAME_CENTER_Y + y1
        };

        if (with_lines) {
            drawLine(
                final_coords[0], final_coords[1],
                final_coords[2], final_coords[3],
                Pixel_bright);
        } else {
            framebufferSet(final_coords[0], final_coords[1], Pixel_bright);
            framebufferSet(final_coords[2], final_coords[3], Pixel_bright);
        }
    }
}

/**
 * @brief Draws the filling of the inner polygon.
 * @detail Filling is accomplished by OR'ing the framebuffer with the fill
 * buffer.
 */
void drawFill(void)
{
    // TODO:
    // implement me
}

/**
 * @brief Draw the arms of the centergon.
 * @detail They are waving and stretching out, until the bars come too close.
 */
void drawCentergonArms(void)
{
	// TODO:
	// save / compute this values somewhere

    // default value that is used if bars are too far away
    const uint8_t max_value = 30;
    uint8_t min_bar_dist = max_value;
    int i;

    // compute minimum distance of all current bars
    for (i=0; i<MAX_BARS; i++) {
        bar_t *b = &game.bars[i];
        if (b->valid && !b->exploding) {
            if (b->dist < min_bar_dist) {
                min_bar_dist = b->dist;
            }
        }
    }

    const int stretch = 3 + min_bar_dist;

    // The effective factor is low pass filtered and mixed with a sine to look
    // more dynamic
    static int factor = 10;
    // TODO:
    // make period shorter, if the less points are needed for level-up
    factor = ((5.0f*factor)/6.0f + (stretch/6.0f));

	if (game.over && game.all_dead && game.all_dead_timer != 0) {
		/*const float eff_factor = ((float)factor) * (1.0f+sin(2.0f*M_PI*game.ticks/15)/2.0f);*/
		factor *= (1.0f+sin(2.0f*M_PI*game.ticks/13)/2.0f);
	}

    // Show a few marker points as the arms
    for (i=0; i<4; i++) {
        uint32_t tmp = 0;
        tmp += factor * pow(i, 2);
        /*tmp += factor*(i*i)/3;*/
        tmp /= 40;
        tmp += i;
        if (game.over && game.all_dead) {
            tmp /= 1+(MAX_DEAD_TIMER - game.all_dead_timer);
        }
		// TODO: make this clearer
        if (!(game.over && game.all_dead && game.all_dead_timer == 0)) {
            tmp += game.inner_radius;
            drawCentergon(tmp, game.shape, false);
        }
    }
}



void calculateRotation(int *x, int *y)
{
    float matrix_a = cos(game.field_rot);
    float matrix_b = -sin(game.field_rot);
    float matrix_c = sin(game.field_rot);
    float matrix_d = cos(game.field_rot);

    float rotated_x = (matrix_a  * (*x) + matrix_b * (*y));
    float rotated_y = (matrix_c *  (*x) + matrix_d * (*y));


    *x = (int)rotated_x;
    *y = (int)rotated_y;
}



