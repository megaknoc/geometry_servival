#ifndef __INCLUDED_GAME_H__
#define __INCLUDED_GAME_H__

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

#ifdef __cplusplus
extern "C" {
#endif

void gameInit(void);
bool gameTick(void);
void gameRender(void);


#ifdef __cplusplus
}
#endif

#endif

