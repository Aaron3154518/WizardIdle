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

// Font identifiers
constexpr auto SMALL_FONT = "smallfont";
constexpr auto LARGE_FONT = "largefont";

// Game class
class Game {
public:
    static void init();
    static void clean();
    static void updateTime();
    static void handleEvents();
    static void update();
    static void render();

    static void setDrawColor(const SDL_Color& c);
    static void resetDrawColor();

    static void setRenderTarget(SDL_Texture* tex);
    static void resetRenderTarget();

    // Getters
    static SDL_Renderer* renderer();
    static const AssetManager& assets();

    static bool running();
    static Timestep ts();
    static Uint32 gameTime();

    static int maxW();
    static int maxH();
    static SDL_Point camera();
    static Rect screen();
    static Rect getAbsRect(const Rect& r);

    static int icon_w, text_h;
};

#endif /* GAME_h */
