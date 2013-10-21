#include "game.h"

bool gameConfigureLevel(void);

struct game_state_t game;

// TODO: get real value
static int init_dist = 60;


/**
 * @brief Generate an array of coordinates for the corners of a ngon.
 * @detail (num_corners+1)*2 corners will be created so it's possilbe to loop
 * through the array with one extra iterations but
 * @detail The supplied buffer must have enough space for one extra coordinate
 * tuple.
 * @detail The coordinates will be based on a local coordinate system with (0,
 * 0) as the center of the ngon.
 * @returns False if the ngon is not supported or parameters are wrong.
 */
bool generateCorners(int8_t *buf, uint8_t radius, uint8_t num_corners)
{
    assert(buf != NULL && radius > 0 && num_corners > 2);

    int8_t *init_buf = buf;
    // phase offset to get nicely aligned shapes
    const float phase = 2.0f*M_PI/(num_corners);
    //const float phase = 0;

    // start with the upper right corner, go clockwise
    int i;
    for (i=0; i<num_corners; i++) {
        const float tmp = phase+(float)i*2.0f*M_PI/(float)num_corners;
        *buf = radius*sin(tmp); buf++;
        *buf = radius*cos(tmp); buf++;
    }

    // append the first corner to the end of the buffer
    *buf = * init_buf;       buf++;
    *buf = *(init_buf+1);   buf++;

    return true;
}

/**
 * @brief Add a new bar at the edge of the playfield.
 * @returns created bar, if valid. NULL on error.
 */
static bar_t *gameAddBar(uint8_t sector, uint8_t width)
{
    // TODO: ignored for now
    /*assert(width > 0);*/
    assert(!(game.num_bars > MAX_BARS));
    if (game.num_bars == MAX_BARS) {
        return NULL;
    }

    int i;
    for (i=0; i<MAX_BARS; i++) {
        bar_t *b = &game.bars[i];
        if (!b->valid) {
            b->valid = true;
            b->sector = sector;
            b->width = width;
            b->dist = init_dist;
            b->exploding = false;
            b->timer = 0;
            game.num_bars++;
            return b;
        }
    }
    return NULL;
}

/**
 * @brief Delete a bar from the game.
 */
static void gameDeleteBar(bar_t *b) {
    assert(b != NULL && b->valid);

    b->valid = false;
    game.num_bars--;
}

void addPointsToAlivePlayers(uint32_t points)
{
	int i;
	for (i=0; i<MAX_PLAYERS; i++) {
		player_t *p = &game.players[i];
		if (p->valid && !p->dead) {
			p->points += points * POINT_MULTIPLIER;
		}
	}
}

/**
 * @brief Transform a bar into an explosion if the bar hits the centergon.
 * @detail May give points to the player if the player is not dead.
 */
static void gameCrushBar(bar_t *b)
{
    if (!game.over) {
		addPointsToAlivePlayers(1);
        game.bars_crushed++;
        // TODO:
        // add one pixel in shape

        gameConfigureLevel();
    }
    b->timer = 11;
    b->exploding = true;
}

static void gameDeleteAllBars(void)
{
    int i;
    for (i=0; i<MAX_BARS; i++) {
        if (game.bars[i].valid) {
            gameDeleteBar(&game.bars[i]);
        }
    }
}
/**
 * @brief Change the level's current polygon.
 * @returns true if the change was successful, false if value is forbidden or
 * the same as the current polygon.
 */
bool gameChangePolygon(uint8_t val)
{
    if (game.shape == val) {
        return false;
    }
    if (val < MIN_SHAPE) {
        return false;
    }

    game.shape = val;
    return true;
}

void changeLevel(void)
{
	/*gameChangePolygon(game.shape+1);*/
	gameChangePolygon((rand() % 5) + MIN_SHAPE);

    gameDeleteAllBars();

    game.bars_crushed = 0;
    // TODO recompute value based on shape
    game.bars_needed = 60;

}

/**
 * @brief Select the speed of the bars depending on the current player/game state.
 * @returns true if the divider changed.
 */
bool gameConfigureLevel(void)
{
	uint8_t next_div;
	switch(game.bars_crushed) {
	case 0:
		next_div = 3;
		break;
	case 10:
		next_div = 2;
		break;
	case 30:
		next_div = 1;
		break;
	case 100:
		changeLevel();
		next_div = 3;
		break;
	default:
		next_div = game.speed_div;
	}

	// select field rotation based on next_div too
	/*game.field_rot_incr = (1+3-next_div)/100.0f;*/
    game.field_rot_incr = 0.03f;

	if (next_div != game.speed_div) {
		game.speed_div = next_div;
		return true;
	} else {
		return false;
	}
}

void initPlayers(void)
{
	int i;
	for (i=0; i<MAX_PLAYERS; i++) {
		player_t *p = &game.players[i];
		p->valid = false;
	}
}

// return false if no more space in buffer for another player
bool addPlayer(void)
{
	int i;
	for (i=0; i<MAX_PLAYERS; i++) {
		player_t *p = &game.players[i];
		if (!p->valid) {
			p->valid = true;
			p->points = 0;
			p->rot = 0.0f;
			p->dead = false;
			p->x = 0;
			p->y = 0;
			return true;
		}
	}
	return false;
}

/**
 * @brief Initialize the game state.
 */
void gameInit(void)
{
    time_t t;
    time(&t);
    srand((unsigned int)t);

    bar_t unused_bar = {.sector=0, .dist=0, .width=0, .valid=false};
    int i;
    for (i=0; i<MAX_BARS; i++) {
        game.bars[i] = unused_bar;
    }

    game.ticks = 0;
    game.speed_div = 1;
    game.num_bars = 0;

	game.field_rot = 0.0f;
	game.field_rot_incr = 0.03f;

	initPlayers();

	addPlayer();
	addPlayer();

    game.bars_crushed = 0;
    game.bars_needed = 100;

    game.over = false;

    game.inner_radius = 8;
    game.shape = MIN_SHAPE;

    input.button_a = false;
    input.button_b = false;

    gameConfigureLevel();
	changeLevel();

    framebufferClear(Pixel_dark);
}

static bool playerCollidesWithBars(player_t *p)
{
	// TODO: use the player's shape here
	assert(p != NULL && p->valid && !p->dead);
#define CHECK_PIXEL(xadd, yadd) \
    pixel = framebufferPixel(p->x+(xadd), p->y+(yadd)); \
    if (*pixel != Pixel_dark) return true; \

    // assumes the player is always a cross
    //  #
    // ###
    //  #
    uint8_t *pixel;

    CHECK_PIXEL( 0,  0);
    CHECK_PIXEL(-1,  0);
    CHECK_PIXEL( 0, -1);
    CHECK_PIXEL( 1,  0);
    CHECK_PIXEL( 0,  1);

    return false;
}

void playerSetDead(player_t *p)
{
	assert(p != NULL && p->valid && !p->dead);
	assert(!game.over);

    /*game.over = true;*/
    p->dead = true;
    p->dead_timer = MAX_DEAD_TIMER;

	// check whether the game is over now
	bool still_alive = false;

	int i;
	for (i=0; i<MAX_PLAYERS; i++) {
		player_t *p = &game.players[i];
		if (p->valid && !p->dead) {
			still_alive = true;
			break;
		}
	}

	if (!still_alive) {
		game.over = true;
		game.all_dead = true;
		game.all_dead_timer = MAX_DEAD_TIMER;
	}
}

/**
 * @brief Render the game objects to the game framebuffer.
 */
void gameRender(void)
{
    // change order
#if 0
    const int max = 100;
    static int div = 0;
    static bool dir_up = true;
    div++;
    if (div && (div % max) == 0) {
        game.shape += dir_up ? 1 : -1;
        if (game.shape == 3 || game.shape> 7) {
            gameDeleteAllBars();
            game.shape = 3;
            //dir_up = !dir_up;
        }

    }
#endif

    framebufferClear(Pixel_dark);

    drawBars(true);

    // Check the player collision now.
    // If there is already a pixel, the player is colliding with the walls.
    if (!game.over) {
		int i;
		for (i=0; i<MAX_PLAYERS; i++) {
			player_t *p = &game.players[i];
			if (p->valid && !p->dead) {
				if (playerCollidesWithBars(p)) {
					playerSetDead(p);
				}
			}
		}
	}

    // now draw the explosion effects, which should not affect the collision
    drawExplodingBars();

    // draw the inner of the polygon
    drawFill();

    // player is currently a cross
	// TODO: draw all players
	int j;
	for (j=0; j<MAX_PLAYERS; j++) {
		player_t *p = &game.players[j];
		if (!p->valid) {
			continue;
		}
		drawPlayer(p);

		int i;
		if (p->dead) {
			for (i=0; i<10; i++) {
				// TODO: check for overflows
				// and use bigger types here
				/*drawCentergon((MAX_DEAD_TIMER-game.dead_timer)+game.inner_radius+10*i, game.shape);*/
				int tmp = MAX_DEAD_TIMER - p->dead_timer;
				tmp += game.inner_radius+10*i;
				drawCentergon((uint8_t) tmp, game.shape, true);
			}
		} else {
			drawCentergon(game.inner_radius, game.shape, true);
		}
	}

    drawCentergonArms();

    // draw the game tick
    // TODO

    // draw the level number
    // TODO

    // draw the points
    // TODO
}

static void movePlayer(player_t *p, bool left_pressed, bool right_pressed)
{
	assert(p != NULL && p->valid);

    uint8_t x,y;

    // TODO
    // XXX nothing static here!
    // need values for multiple players
    if (left_pressed) {
        p->rot++;
    } else if (right_pressed) {
        p->rot--;
    }

    x = GAME_CENTER_X;
    y = GAME_CENTER_Y;

    const int r = game.inner_radius+2;
    const float div = 5.0;
    x += r*sin(p->rot/div);
    y += r*cos(p->rot/div);

	p->x = x;
	p->y = y;
}

/**
 * @brief Check whether placing a bar in sector s makes the game impossible.
 * @returns true if it is ok, false if the player cannot escape when placing
 * that bar.
 */
bool isSectorOk(uint8_t s)
{
    uint8_t max_distance = 0;

    int i;
    for (i = 0; i < MAX_BARS; i++) {
        bar_t *currBar = &game.bars[i];
        if (currBar->valid) {
            max_distance = currBar->dist > max_distance ? currBar->dist : max_distance;
        }
    }

    if (init_dist - max_distance < 7) {   // 7 should be an appropriate value
        return false;
    }

    return true;
}

/**
 * @brief Advance the game state.
 * @returns true if game should be reset.
 */
bool gameTick(void)
{
	printf("crushed: %d\n", game.bars_crushed);
	int i;
	for (i=0; i<MAX_PLAYERS; i++) {
		player_t *p = &game.players[i];
		if (!p->valid) {
			continue;
		}
		// tick dead players
		if (p->dead) {
			if (p->dead_timer > 0) {
				p->dead_timer--;
			}
		} else {
			// move the player if a button is pressed
            if (i == 0) {
                movePlayer(p, input.button_a, input.button_b);
            } else if (i == 1) {
                movePlayer(p, input.button_p2_a, input.button_p2_b);
            } else {
                // more than 2 players not yet supported
                assert(false);
            }
		}
	}

	if (game.over && game.all_dead) {
		if (game.all_dead_timer > 0) {
			game.all_dead_timer--;
		}
	}

    // check whether the player collides with bars
    // TODO

    // move each bar closer to the center
    for (i=0; i<MAX_BARS; i++) {
        bar_t *b = &game.bars[i];
        if (b->valid) {
            if (b->exploding) {
                // advance explosion of bar
                if (b->timer > 0) {
                    b->timer--;
                } else {
                    gameDeleteBar(b);
                }
            } else {
                // explode bars that have just reached the center
                if (b->dist <= game.inner_radius) {
                    gameCrushBar(b);
                    continue;
                }
                // move other bars closer to the center
                if (game.ticks && (game.ticks % game.speed_div) == 0) {
                    b->dist -= 1;
                }
            }
        }
    }

    // add new bars until there are enough on the playfield
    if (!game.over) {
        if (game.num_bars < MAX_BARS) {
            if (rand() % 180 < 13 + 8*(game.shape-MIN_SHAPE)) {
            /*if (rand() % 180 < (10 + 2*(MAX_BAR_SPEED_DIVIDER-game.speed_div))) {*/
                // TODO: randomize + patterns
                uint8_t s = rand() % game.shape;
                if (isSectorOk(s)) {
                    gameAddBar(s, 0);
                }
            }
        }
    }

    // rotate the playfield
	game.field_rot += game.field_rot_incr;		//Todo: Fix overflow

	// 1/250th chance of changing the rotation direction
	if (rand() % 250 < 1) {
		game.field_rot_incr *= -1;
	}

    // move the plafield nearer to the player
    // TODO

    // change direction and amount of playfield rotation from time to time
    // TODO
    game.ticks++;

    if (game.over && input.restart) {
        return false;
    }
    return true;
}

