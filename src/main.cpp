#include "graphics.h"
#include "game.h"
#include "config.h"
 
// The custom callback function that the library calls 
// to check for and set the current application state.
void update(float ms)
{
    Game* game = reinterpret_cast<Game*>(graphics::getUserData());
    game->update();
}
 
// The window content drawing function.
void draw()
{
    Game* game = reinterpret_cast<Game*>(graphics::getUserData());
    game->draw();
}
 
int main() //h main ekleise, h leitoyrgikothta ths einai komple
{
    Game mygame;

    graphics::createWindow(WINDOWS_WIDTH, WINDOW_HEIGHT, "FIGHTERS COLLIDE");

    graphics::setUserData(&mygame);
 
    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);
 
    graphics::setCanvasSize(CANVAS_WIDTH, CANVAS_HEIGHT);
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);
    
    mygame.init();
    graphics::startMessageLoop();
 
    return 0;
}