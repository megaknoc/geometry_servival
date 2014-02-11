#ifndef __INCLUDED_DEMO_H__
#define __INCLUDED_DEMO_H__

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "constants.h"
#include "draw.h"
#include "framebuffer.h"
#include "game_types.h"
#include "input.h"
#include "util.h"
/**
 * Graphic demo for testing and playing with drawing functions.
 * This is not part of the game and is just for testing/developing.
 */

#ifdef __cplusplus
extern "C" {
#endif

void demoInit(void);
void demoRender(void);
bool demoTick(void);

#ifdef __cplusplus
}
#endif

#endif
