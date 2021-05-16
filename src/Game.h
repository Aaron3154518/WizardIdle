#ifndef GAME_h
#define GAME_h

#include <vector>
#include <unordered_map>

#include "dirent.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Tools/Rect.h"
#include "Tools/Number.h"
#include "Tools/Tools.h"
#include "AssetManager.h"
#include "Renderer.h"
#include "Updater.h"
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
    SDL_Renderer* sdlRenderer;
	Timestep ts;
    Event event; // TODO: make sure event includes time step
public:
    Game(const Game&) = delete;
    void operator=(const Game&) = delete;

    static Game& get();

	// TODO: These shouldn't be public
    void init();
    void clean();
	void tick();

    void setDrawColor(const SDL_Color& c);
    void resetDrawColor();

    void setRenderTarget(SDL_Texture* tex);
    void resetRenderTarget();

	Uint32 gameTime = 0;
    int maxW = 0, maxH = 0;
    int icon_w = 0, text_h = 0;
    SDL_Point camera;
    Rect screen;

    AssetManager assets;
	Renderer renderer;
	Updater updater;
	WizardInfo wizards;


    // Getters
    bool running() { return mRunning; }
    Rect getAbsRect(const Rect& r);
};

#endif /* GAME_h */
