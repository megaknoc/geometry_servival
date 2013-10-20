#include <iostream>
#include <math.h>

#include <SFML/Window.hpp>
//#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include "simulator.hpp"
#include "framebuffer.h"
#include "game.h"

int main()
{
    Simulator s;

    do {
        gameInit();

        while (s.isRunning())
        {
            s.processInput();

            bool go_on = gameTick();
            if (!go_on) {
                break;
            }
            gameRender();

            s.calculateFramebuffer();
            s.showFramebuffer();
        }
    } while(s.isRunning());

    return 0;
}

