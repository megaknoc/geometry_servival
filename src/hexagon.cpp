#include <iostream>
#include <math.h>

#include <SFML/Window.hpp>
//#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include "simulator.hpp"
#include "framebuffer.h"
#include "demo.h"
#include "game.h"

void (*initFunction)(void)   = NULL;
bool (*tickFunction)(void)   = NULL;
void (*renderFunction)(void) = NULL;

void configureForGameOrDemo(bool game)
{
    if (game) {
#ifdef SIMULATION
        printf("*** configured hexagon for GAME\n");
#endif
        initFunction    = gameInit;
        renderFunction  = gameRender;
        tickFunction    = gameTick;
    } else {
#ifdef SIMULATION
        printf("*** configured hexagon for DEMO\n");
#endif
        initFunction    = demoInit;
        renderFunction  = demoRender;
        tickFunction    = demoTick;
    }
}

int main()
{
    Simulator s;

    // select demo or game
    configureForGameOrDemo(false);

    do {
        initFunction();
        s.calculateFramebuffer(true);

        while (s.isRunning()) {
            s.processInput();

            bool go_on = tickFunction();
            if (!go_on) {
                break;
            }
            renderFunction();

            s.calculateFramebuffer(false);
            s.showFramebuffer();
        }
    } while(s.isRunning());

    return 0;
}

