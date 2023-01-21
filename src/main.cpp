#include "Game.h"

const GLuint SCR_WIDTH  = 1080;
const GLuint SCR_HEIGHT = 1080;

int main()
{
    Prototype awesome(SCR_WIDTH, SCR_HEIGHT);
    awesome.start();
    awesome.run();
    return 0;
}
