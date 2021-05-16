#define SDL_MAIN_HANDLED

#include <SDL.h>

#include "Game.h"

int main(int argc, char* argv[]) {
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Game& game = Game::get();
    game.init();

    while (game.running()) {
        game.tick();

        int dt = SDL_GetTicks() - game.gameTime;
        if (dt < frameDelay) {
            SDL_Delay(frameDelay - dt);
        }
    }
    
    game.clean();
    return 0;
}
