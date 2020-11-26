#ifndef GAME_h
#define GAME_h

#include <vector>
#include <unordered_map>

#include "dirent.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Definitions.h"
#include "Rect.h"
#include "Number.h"
#include "Tools.h"
#include "AssetManager.h"
#include "Wizards/WizardData.h"

// Font identifiers
constexpr auto SMALL_FONT = "smallfont";
constexpr auto LARGE_FONT = "largefont";

// Game class
class Game {
private:
    Game() = default;
    ~Game() = default;

    bool mRunning = false, mDragging = false;
    SDL_Window* mWindow;
public:
    Game(const Game&) = delete;
    void operator=(const Game&) = delete;

    static Game& get();

    void init();
    void clean();
    void updateTime();
    void handleEvents();
    void update();
    void render();

    void setDrawColor(const SDL_Color& c);
    void resetDrawColor();

    void setRenderTarget(SDL_Texture* tex);
    void resetRenderTarget();

    int maxW = 0, maxH = 0;
    int icon_w = 0, text_h = 0;
    SDL_Point camera;
    Rect screen;
    SDL_Renderer* renderer;
    AssetManager assets;

    Uint32 gameTime = 0;
    Timestep ts;
    Event event;
    WizardData wizards;

    // Getters
    bool running() { return mRunning; }
    Rect getAbsRect(const Rect& r);
};

#endif /* GAME_h */
