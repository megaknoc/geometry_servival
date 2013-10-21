#ifndef __INCLUDED_CONSTANTS_H__
#define __INCLUDED_CONSTANTS_H__


#define SIMULATION

#define FRAMEBUFFER_WIDTH           (96)
#define FRAMEBUFFER_HEIGHT          (68)
#define FRAMEBUFFER_TOTAL_PIXELS    (FRAMEBUFFER_WIDTH*FRAMEBUFFER_HEIGHT)

//#define GAME_AREA_SIZE              (68)
//#define GAME_CENTER_X               (GAME_AREA_SIZE/2)
//#define GAME_CENTER_Y               (GAME_AREA_SIZE/2)
#define GAME_CENTER_X               (FRAMEBUFFER_WIDTH  / 2)
#define GAME_CENTER_Y               (FRAMEBUFFER_HEIGHT / 2)

// pixel padding is not yet properly implemented
#define PIXEL_PADDING               0
#define PIXEL_SIZE                  4

// maximum amount of bars that can be created at a time
#define MAX_BARS                    200
#define MAX_PLAYERS                 10

// minimum shape (less than triangle makes not much sense)
#define MIN_SHAPE                   3

#define MAX_BAR_SPEED_DIVIDER       5

#define MAX_DEAD_TIMER              20
#define POINT_MULTIPLIER            1

//const uint8_t Center_x = GAME_AREA_SIZE/2;
//const uint8_t Center_y = GAME_AREA_SIZE/2;

#endif

