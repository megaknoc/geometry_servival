#include "framebuffer.h"

struct framebuffer_t framebuffer;

/**
 * @brief Get a pointer to the pixel in the framebuffer at x/y coordinates.
 */
uint8_t *framebufferPixel(uint8_t x, uint8_t y)
{
    assert(x < FRAMEBUFFER_WIDTH && y < FRAMEBUFFER_HEIGHT);
    return &framebuffer.data[x+FRAMEBUFFER_WIDTH*y];
}

/**
 * @brief Set a pixel in the framebuffer.
 */
void framebufferSet(uint8_t x, uint8_t y, uint8_t val)
{
    if (x >= FRAMEBUFFER_WIDTH || y >= FRAMEBUFFER_HEIGHT) {
        return;
    }
    uint8_t *pixel = framebufferPixel(x, y);
    *pixel = val;
}

/**
 * @brief Clear the framebuffer (set all pixels to a predefined value)
 */
void framebufferClear(uint8_t val)
{
    int x, y;
    for (x=0; x<FRAMEBUFFER_WIDTH; x++) {
        for (y=0; y<FRAMEBUFFER_HEIGHT; y++) {
            framebufferSet(x, y, val);
        }
    }
}

