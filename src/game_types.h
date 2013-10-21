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

#define PLAYER_SHAPE_HEIGHT     4
#define PLAYER_SHAPE_WIDTH      4
/**
 * For simplicity, a 4x4 (pixel) shape is assumed.
 * ATM one bytes is used for a pixel.
 * Set bits are solid white pixels, unset bits are ignored when
 * drawing and checking collisio.
 */
typedef struct player_shape_t {
    uint8_t data[PLAYER_SHAPE_HEIGHT*PLAYER_SHAPE_WIDTH];
} player_shape_t;


typedef struct player_t {
    uint32_t    points;     // Player gets one point for each crushed bar.
    float       rot;        // Rotation (CW) off the player in radian.
                            // 0      : at the top of the hexagon.
                            // ..2*pi : somewhere clockwise around the circle
    uint8_t     x;          // x-coordinate of center of player
    uint8_t     y;          // y-coordinate of center of player
    bool        dead;       // If true, player collided with bars and is dead
    uint8_t     dead_timer; // How long is the player dead
    bool        valid;      // true, if the buffer position for this player is used
} player_t;

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
    uint32_t bars_crushed;  // Bars crushed this level. Reset on shape change.
    uint32_t bars_needed ;  // How many crushed bars are needed to advance shape
                            // Must be recomputed to the amount of pixels that
                            // fills the current centergon each time shape
                            // changes

    float field_rot;        // rotation of the field;
	float field_rot_incr;;	// incrementation of field_rot each tick

    uint8_t num_bars;       // number of bars on the playfield
    uint8_t shape;          // order of the current level's ngon: 3=tri, 4=quad, ...
    uint8_t inner_radius;   // inner radius
    uint8_t speed_div;      // divider for the speed of bars
    bool over;              // game is over
	bool all_dead;			// flag indicating that all players are dead
	uint8_t all_dead_timer; // timer that starts when all players died
    bar_t bars[MAX_BARS];   // buffer for all bars
	player_t players[MAX_PLAYERS]; // all players
} game;

#endif

