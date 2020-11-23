#define SDL_MAIN_HANDLED

#include <SDL.h>

#include "Game.h"
#include "Number_unittest.cpp"

int main(int argc, char* argv[]) {
    if (false) {
        runTests();
        return 0;
    }

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Game::init();

    while (Game::running()) {
        Game::updateTime();
        Game::handleEvents();
        Game::update();
        Game::render();

        int dt = SDL_GetTicks() - Game::gameTime();
        if (dt < frameDelay) {
            SDL_Delay(frameDelay - dt);
        }
    }
    
    Game::clean();
    return 0;
}
