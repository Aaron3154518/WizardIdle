#include "Game.h"
#include "Wizards/WizardContext.h"

struct GameStruct {
    friend class Game;
private:
    int mMaxW = 0, mMaxH = 0;
    SDL_Point mCamera;
    Rect mScreen;
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    AssetManager mAssets;

    bool mRunning = false, mDragging = false;
    Uint32 mGameTime = 0;
    Timestep mTS;
    Event mEvent;
};

static GameStruct game;
int Game::icon_w = 0;

void Game::init() {
    // No double initializing!
    if (game.mRunning) { return; }
    int flags = SDL_WINDOW_RESIZABLE;
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "SDL Initialized" << std::endl;

        Rect temp;
        SDL_GetDisplayUsableBounds(0, &temp);
        game.mMaxW = temp.w; game.mMaxH = temp.h;

        game.mScreen = Rect(0, 0, game.mMaxW / 2, game.mMaxH / 2);
        game.mCamera = { (int)game.mScreen.cX(), (int)game.mScreen.cY() };
        std::cout << "Screen Set: " << game.mScreen << std::endl;
        game.mWindow = SDL_CreateWindow("Wizard Idle", SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, game.mScreen.w, game.mScreen.h, flags);

        game.mRenderer = SDL_CreateRenderer(game.mWindow, -1, 0);
        if (game.mRenderer) {
            SDL_SetRenderDrawColor(game.mRenderer, 255, 255, 255, 255);
        }

        game.mRunning = true;
    }

    if (TTF_Init() == 0) {
        std::cout << "SDL_TTF Initialized" << std::endl;
    }
    else {
        game.mRunning = false;
        return;
    }

    game.mAssets = AssetManager();

    game.mAssets.loadFont(SMALL_FONT, "assets/times.ttf", -1, game.mScreen.h / 25);
    game.mAssets.loadFont(LARGE_FONT, "assets/times.ttf", -1, game.mScreen.h / 10);

    DIR* assetsDir = opendir("assets/");
    struct dirent* en;
    if (assetsDir) {
        while ((en = readdir(assetsDir)) != NULL) {
            std::string name = en->d_name;
            size_t idx = name.find_last_of(".");
            if (idx != std::string::npos && name.substr(idx) == ".png") {
                game.mAssets.loadAsset(name.substr(0, idx), ("assets/" + name).c_str());
            }
        }
        closedir(assetsDir);
    }

    icon_w = (int)(fmin(game.mMaxW, game.mMaxH) / 15);
    Wizards::init();
}
void Game::clean() {
    // If we call clean, the game must end
    game.mRunning = false;
    // No double cleaning!
    if (game.mWindow == nullptr || game.mRenderer == nullptr) { return; }
    SDL_DestroyWindow(game.mWindow);
    SDL_DestroyRenderer(game.mRenderer);
    SDL_Quit();
    std::cout << "Renderer Cleaned" << std::endl;
}
void Game::updateTime() {
    game.mTS = Timestep(SDL_GetTicks() - game.mGameTime);
    game.mGameTime += game.mTS.GetMilliseconds();
}
void Game::update() {
    //std::cout << "Update" << std::endl;
    Wizards::update(game.mTS);
}
void Game::handleEvents() {
    //std::cout << "Events" << std::endl;
    // Update buttons and mouse
    int dx, dy;
    SDL_GetMouseState(&dx, &dy);
    game.mEvent.mouse = { dx, dy };
    game.mEvent.globalMouse = { dx - game.mCamera.x, dy - game.mCamera.y };
    game.mEvent.mouseDx = 0; game.mEvent.mouseDy = 0;
    game.mEvent.update(game.mTS);
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_QUIT:
            game.mRunning = false;
            return;
        case SDL_WINDOWEVENT:
            if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                double oldCX = game.mScreen.cX(), oldCY = game.mScreen.cY();
                game.mScreen.resize(e.window.data1, e.window.data2, false);
                game.mCamera.x += (int)(game.mScreen.cX() - oldCX);
                game.mCamera.y += (int)(game.mScreen.cY() - oldCY);
                return;
            }
            break;
        default:
            game.mEvent.update(e);
            break;
        }
    }
    game.mEvent.handled = false;
    if (!game.mDragging) {
        Wizards::handleEvent(game.mEvent);
    }
    if (!game.mEvent.handled) {
        if (game.mEvent.left.clicked) {
            game.mDragging = false;
        }
        if (game.mEvent.left.pressed || game.mDragging) {
            if (game.mEvent.mouseDx != 0 || game.mEvent.mouseDy != 0) {
                game.mDragging = true;
                game.mCamera.x = (int)fmin(fmax(game.mCamera.x + game.mEvent.mouseDx, 0), game.mMaxW);
                game.mCamera.y = (int)fmin(fmax(game.mCamera.y + game.mEvent.mouseDy, 0), game.mMaxH);
            }
        }
    }
}
void Game::render() {
    //std::cout << "Renderer" << std::endl;
    SDL_RenderClear(game.mRenderer);
    Wizards::render();
    SDL_RenderPresent(game.mRenderer);
}

void Game::setDrawColor(const SDL_Color& c) {
    SDL_SetRenderDrawColor(game.mRenderer, c.r, c.g, c.b, c.a);
}
void Game::resetDrawColor() {
    SDL_SetRenderDrawColor(game.mRenderer, 255, 255, 255, 255);
}
void Game::setRenderTarget(SDL_Texture* tex) {
    if (SDL_SetRenderTarget(game.mRenderer, tex) != 0) {
        std::cout << "Unable to set render target" << std::endl;
    }
}
void Game::resetRenderTarget() {
    SDL_SetRenderTarget(game.mRenderer, NULL);
    resetDrawColor();
}

SDL_Renderer* Game::renderer() { return game.mRenderer; }
const AssetManager& Game::assets() { return game.mAssets; };

bool Game::running() { return game.mRunning; }
Timestep Game::ts() { return game.mTS; }
Uint32 Game::gameTime() { return game.mGameTime; }

int Game::maxW() { return game.mMaxW; }
int Game::maxH() { return game.mMaxH; }
SDL_Point Game::camera() { return game.mCamera; }
Rect Game::screen() { return game.mScreen; }
Rect Game::getAbsRect(const Rect& r) {
    return Rect(r.x + game.mCamera.x, r.y + game.mCamera.y, r.w, r.h);
}
