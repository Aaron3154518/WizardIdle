#define SDL_MAIN_HANDLED

#include <SDL.h>

#include "Game.h"
#include "Number_unittest.cpp"

Game game;

int main(int argc, char* argv[]) {
    if (false) {
        runTests();
        return 0;
    }

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    game.init();

    while (game.running()) {
        game.updateTime();
        game.handleEvents();
        game.update();
        game.render();

        int dt = SDL_GetTicks() - game.gameTime();
        if (dt < frameDelay) {
            SDL_Delay(frameDelay - dt);
        }
    }
    
    game.clean();
    return 0;
}