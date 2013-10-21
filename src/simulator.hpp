#ifndef __INCLUDED_SIMULATOR_HPP__
#define __INCLUDED_SIMULATOR_HPP__

#include <iostream>
#include <math.h>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include "constants.h"
#include "input.h"
#include "framebuffer.h"
#include "game.h"

class Simulator {
    private:
    sf::RenderWindow window;
    sf::Shader shader;
    bool running;
    sf::VertexArray pixel_quads; // pixkels are represented by a quad array

    public:
    // TODO: get useful arguments
    Simulator();
    ~Simulator();

    void drawRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const sf::Color color);
    void showFramebuffer(void);
    bool isRunning(void) { return running; };
    void processInput(void);
    void calculateFramebuffer(bool force_pixels);
    sf::Vertex* getPixelVertices(uint8_t x, uint8_t y);
};


#endif
