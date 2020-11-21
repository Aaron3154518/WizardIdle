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

// Forward Declarations
class AssetManager;

// Game class
class Game {
public:
    Game() = default;
    ~Game() = default;

    void init();
    void clean();
    void updateTime();
    void handleEvents();
    void update();
    void render();

    void setDrawColor(const SDL_Color& c);
    void resetDrawColor();

    // Getters
    SDL_Renderer* renderer() const;
    const AssetManager& assets() const;

    bool running() const;
    Uint32 gameTime() const;

    int maxW() const;
    int maxH() const;
    SDL_Point camera() const;
    Rect screen() const;
    Rect getAbsRect(const Rect& r) const;

    static int icon_w;
};

#endif /* GAME_h */
