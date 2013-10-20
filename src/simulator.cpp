#include "simulator.hpp"

/**
 * @brief Draw a filled rectangle on the simulator screen.
 */
void Simulator::drawRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const sf::Color color)
{
    sf::RectangleShape r(sf::Vector2f(x, y));
    r.setPosition(w, h);
    r.setFillColor(color);
    //window.draw(r);
    // XXX
    // window.draw(r);
}


void Simulator::calculateFramebuffer(void)
{
        int x,y;
        for (x=0; x<FRAMEBUFFER_WIDTH; x++) {
            for (y=0; y<FRAMEBUFFER_HEIGHT; y++) {
                uint8_t *px = framebufferPixel(x, y);
                const sf::Color clr = *px > 0 ? sf::Color::White : sf::Color::Black;
                sf::Vertex *v = getPixelVertices(x, y);
                int j;
                for (j=0; j<4; j++) {
                    v[j].color = clr;
                }
            }
        }
}

void Simulator::showFramebuffer(void)
{
        // clear the buffers
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // set bg color
        window.clear(sf::Color::Green);
        //window.clear(sf::Color::Black);
        //window.clear(sf::Color::White);

        // render each framebuffer pixel
        // XXX
        // TODO
#if 0
        const uint8_t px_size = 8;
        int x,y;
        for (x=0; x<FRAMEBUFFER_WIDTH; x++) {
            for (y=0; y<FRAMEBUFFER_HEIGHT; y++) {
                uint8_t *px = framebufferPixel(x, y);
                //const sf::Color c = *px < (0xff/2) ? sf::Color(0, 0, 0) : sf::Color(0xff, 0xff, 0xff);
                const sf::Color c = sf::Color(0xff, 0xff, 0xff);
                // colorize the pixels

                // now, draw a square for each pixel
                //drawRectangle(x*px_size, y*px_size, px_size, px_size, c);
            }
        }
#endif

        // draw the vertex array
        window.draw(pixel_quads);

        //glMatrixMode(GL_PROJECTION);
        //glMatrixMode(GL_MODELVIEW);
        //glViewport(0, 0, (GLsizei) WIDTH, (GLsizei) HEIGHT);
        //glLoadIdentity();
        //gluPerspective(90.0, 1.0, 0.001, 1000.0);

        // move camera away
        //glTranslatef(0.0f, 0.0f, -10.0f*((1+cos(tick/20.0f))/2.0f));
        //glTranslatef(0.0f, 0.0f, -0.5f);
        //glTranslatef(0.0f, 0.0f, -10.0f);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //glDisable(GL_CULL_FACE);

        //glClearDepth(0.0f);
        //glPushMatrix();
        //shader.setParameter("tick", (float)tick);

        // bind the shader
        //sf::Shader::bind(&shader);

        // draw the monster: one sphere with dents
        // TODO: draw sth.

        //glPopMatrix();

        // bind no shader
        //sf::Shader::bind(NULL);
        window.display();
}

Simulator::Simulator() : window(
    sf::VideoMode(FRAMEBUFFER_WIDTH*PIXEL_SIZE, FRAMEBUFFER_HEIGHT*PIXEL_SIZE),
    "float_please",
    sf::Style::Default,
    sf::ContextSettings(32)),

    pixel_quads(sf::Quads, 4*FRAMEBUFFER_TOTAL_PIXELS)
{
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(25);

    // load both shaders
    //if (!shader.loadFromFile("data/shader/shader.vert", "data/shader/shader.frag")) {
        //std::cerr<<"Could not load shaders from file!"<<::std::endl;
        //exit(-1);
    //}
    //shader.setParameter("texture", sf::Shader::CurrentTexture);

    // define it as a rectangle, located at (10, 10) and with size 100x100
    const int h = PIXEL_SIZE;
    const int w = h;

    //const int game_area_margin_x = 1;
    //const int game_area_margin_y = 15;

    int x, y;
    for (y=0; y<FRAMEBUFFER_HEIGHT; y++) {
        for (x=0; x<FRAMEBUFFER_WIDTH; x++) {
            const int i = 4 * (x + y*FRAMEBUFFER_WIDTH);

            const int padx = PIXEL_PADDING;
            const int pady = padx;
            const int ox = x * (w + padx);
            const int oy = y * (h + pady);

            pixel_quads[i+0].position = sf::Vector2f(ox,      oy);
            pixel_quads[i+1].position = sf::Vector2f(ox+w,    oy);
            pixel_quads[i+2].position = sf::Vector2f(ox+w,    oy+h);
            pixel_quads[i+3].position = sf::Vector2f(ox,      oy+h);

            sf::Color c = sf::Color(0, 0, (x + y) & 1 ? 0xff : 0x00);

            pixel_quads[i+0].color = c;
            pixel_quads[i+1].color = c;
            pixel_quads[i+2].color = c;
            pixel_quads[i+3].color = c;
        }
    }

    // run the main loop
    running = true;
}

sf::Vertex* Simulator::getPixelVertices(uint8_t x, uint8_t y)
{
    assert(x < FRAMEBUFFER_WIDTH && y < FRAMEBUFFER_HEIGHT);
    const int i = 4 * (x + y*FRAMEBUFFER_WIDTH);

    assert(i < (4 * FRAMEBUFFER_TOTAL_PIXELS));
    return &pixel_quads[i];
}

Simulator::~Simulator()
{
}

void Simulator::processInput(void)
{
    // handle events
    sf::Event event;
    while (window.pollEvent(event))
    {
        // TODO: add escape
        if (event.type == sf::Event::Closed)
        {
            // end the program
            running = false;
        }
        else if (event.type == sf::Event::Resized)
        {
            // adjust the viewport when the window is resized
            glViewport(0, 0, event.size.width, event.size.height);
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        running = false;
    }

    // use convenient static functions
    input.button_a = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    input.button_b = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    input.restart  = sf::Keyboard::isKeyPressed(sf::Keyboard::R);
}

