#include "demo.h"

void demoInit(void)
{
    // nothing for now
}

static int demoState = 0;
static int demoStateMax = 30;
static int demoStateCounter = 0;
static int demoStateDiv = 1;

static void renderArrowLikeForm(int x0, int y0)
{
    float blend = (demoState < demoStateMax)
        ? (demoState/(float) (demoStateMax-1))
        : 1.0f;

    int d = 20 * blend;

    // arrow like form
    drawLine(x0, y0, x0+d, y0+d, Pixel_bright);
    drawLine(x0, y0, x0+d, y0, Pixel_bright);
    drawLine(x0, y0, x0, y0+d, Pixel_bright);
}

static void renderRectangle(int base, int width, float phase)
{
    int d;
    int x0, y0;
    d = (width/2) * (1 + sin(
        phase +
        (2.0f*M_PI*demoState) / (demoStateMax-1)));

    x0 = base;
    y0 = base;
    drawLine(x0, y0, x0+d, y0, Pixel_bright);

    x0 = base + width;
    y0 = base;
    drawLine(x0, y0, x0, y0+d, Pixel_bright);

    x0 = base + width;
    y0 = base + width;
    drawLine(x0, y0, x0-d, y0, Pixel_bright);

    x0 = base;
    y0 = base + width;
    drawLine(x0, y0-d, x0, y0, Pixel_bright);
}

static void renderRectangles(void)
{
    int num = 10;
    int i;
    for (i=0; i<num; i++) {
        const int offset = 4;
        int base = 10;
        int width = 10;

        float phase = 2.0f*M_PI*i/(float)num;
        renderRectangle(base + i*offset, width, phase);
    }
}

static int randInt(int from, int to)
{
    assert(from <= to);
    int range = to - from;
    return from + (rand() % range);
}

static int randPosNeg(range)
{
    if (range < 0) {
        range *= -1;
    }
    return randInt(-range, range);
}

void demoRender(void)
{
    framebufferClear(Pixel_dark);

    /*renderArrowLikeForm(50, 10);*/
    /*renderRectangles();*/

    drawBezier(
        50, 5, 30, -10,
        80, 60, -50, 0,
        Pixel_bright);

    int x = 50;
    int y = 50;

    int i;
    for (i=0; i<1; i++) {
        drawBezier(
            0, 0,
            rand() % x, rand() % y,
            40, 40,
            -rand() % x, -rand() % y,
            Pixel_bright);
    }


    /*int i;*/
    /*for (i=0; i<3; i++) {*/
        /*int a, b, c, d;*/
        /*a = rand() % x;*/
        /*b = rand() % y,*/
        /*c = -rand() % x;*/
        /*d = -rand() % y;*/

        /*drawBezier(*/
            /*0+2*i, 0+2*i, a, b,*/
            /*40-2*i, 40-2*i, c, d*/
            /*);*/
    /*}*/
}

bool demoTick(void)
{
    demoStateCounter++;
    if (demoStateCounter >= demoStateDiv) {
        demoStateCounter = 0;

        /*if (demoState < demoStateMax) {*/
        /*}*/
        // advance state
        demoState++;
    }

    // TODO: let the demo end after some time
    return true;
}
