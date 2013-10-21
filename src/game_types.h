#ifndef __INCLUDED_GAME_TYPES_H__
#define __INCLUDED_GAME_TYPES_H__

#include <stdint.h>
#include <assert.h>
#include "constants.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

/**
 * Position of a bar on the playfield.
 */
typedef struct bar_t {
    uint8_t sector;     // sector in the hexagon in which the bar lies
    uint8_t dist;       // distance of the bar from the center
    uint8_t width;      // width of the bar (extends to the outside)
    uint8_t timer;      // timer for the progress of an exploding bar
    uint8_t valid:1;    // flag: this bar is used
    uint8_t exploding:1;// flag: this bar is currently exploding
} bar_t;

extern struct game_state_t {
    uint32_t ticks;         // Game tick
    uint32_t points;        // Player gets one point for each crushed bar.
    uint32_t bars_crushed;  // Bars crushed this level. Reset on shape change.
    uint32_t bars_needed ;  // How many crushed bars are needed to advance shape
                            // Must be recomputed to the amount of pixels that
                            // fills the current centergon each time shape
                            // changes

    float player_rot;       // rotation (CW) of the player in radian.
                            // 0    : at the top of the hexagon.
                            // 2*pi : at the top of the hexagon.
    float field_rot;        // rotation of the field;

    uint8_t num_bars;       // number of bars on the playfield
    uint8_t shape;          // order of the current level's ngon: 3=tri, 4=quad, ...
    uint8_t player_x;       // x-coordinate of player
    uint8_t player_y;
    uint8_t inner_radius;   // inner radius
    uint8_t speed_div;      // divider for the speed of bars
    bool over;              // game is over
    bool dead;              // if true, player collided with bars
    uint8_t dead_timer;     // how long is the player dead
    bar_t bars[MAX_BARS];   // buffer for all bars
} game;

#endif

