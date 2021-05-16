#include "Game.h"

Game& Game::get() {
    static Game game;
    return game;
}

void Game::init() {
    if (mRunning) { return; }
    int flags = SDL_WINDOW_RESIZABLE;
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "SDL Initialized" << std::endl;

        Rect temp;
        SDL_GetDisplayUsableBounds(0, &temp);
        maxW = temp.w; maxH = temp.h;

        screen = Rect(0, 0, maxW / 2, maxH / 2);
        camera = { (int)screen.cX(), (int)screen.cY() };
        std::cout << "Screen Set: " << screen << std::endl;
        mWindow = SDL_CreateWindow("Wizard Idle", SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, screen.w, screen.h, flags);

        renderer = SDL_CreateRenderer(mWindow, -1, 0);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }

        mRunning = true;
    }

    if (TTF_Init() == 0) {
        std::cout << "SDL_TTF Initialized" << std::endl;
    }
    else {
        mRunning = false;
        return;
    }

    assets = AssetManager();

    text_h = screen.h / 25;
    assets.loadFont(SMALL_FONT, "assets/times.ttf", -1, text_h);
    assets.loadFont(LARGE_FONT, "assets/times.ttf", -1, screen.h / 10);

    DIR* assetsDir = opendir("assets/");
    struct dirent* en;
    if (assetsDir) {
        while ((en = readdir(assetsDir)) != NULL) {
            std::string name = en->d_name;
            size_t idx = name.find_last_of(".");
            if (idx != std::string::npos && name.substr(idx) == ".png") {
                assets.loadAsset(name.substr(0, idx), ("assets/" + name).c_str());
            }
        }
        closedir(assetsDir);
    }

    icon_w = (int)(fmin(maxW, maxH) / 15);
    wizards.init();
}
void Game::clean() {
    mRunning = false;
    wizards.clean();
    SDL_DestroyWindow(mWindow);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Renderer Cleaned" << std::endl;
}

void Game::tick() {
	// Update Objects
    //std::cout << "Update" << std::endl;
    ts = Timestep(SDL_GetTicks() - gameTime);
    gameTime += ts.GetMilliseconds();
    updater.update(ts);

	// Handle Events
    //std::cout << "Events" << std::endl;
    // Update buttons and mouse
    int dx, dy;
    SDL_GetMouseState(&dx, &dy);
    event.mouse = { dx, dy };
    event.globalMouse = { dx - camera.x, dy - camera.y };
    event.mouseDx = 0; event.mouseDy = 0;
    event.update(ts);
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_QUIT:
            mRunning = false;
            return;
        case SDL_WINDOWEVENT:
            if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                double oldCX = screen.cX(), oldCY = screen.cY();
                screen.resize(e.window.data1, e.window.data2, false);
                camera.x += (int)(screen.cX() - oldCX);
                camera.y += (int)(screen.cY() - oldCY);
                return;
            }
            break;
        default:
            event.update(e);
            break;
        }
    }
    event.handled = false;
    if (!mDragging) {
        Updater.handleEvents(event);
/*        if (!event.handled && event.left.clicked) {
            wizards.crystal.magic *= 10;
        }*/
    }
    if (!event.handled) {
        if (event.left.clicked) {
            mDragging = false;
        }
        if (event.left.pressed || mDragging) {
            if (event.mouseDx != 0 || event.mouseDy != 0) {
                mDragging = true;
                camera.x = (int)fmin(fmax(camera.x + event.mouseDx, 0), maxW);
                camera.y = (int)fmin(fmax(camera.y + event.mouseDy, 0), maxH);
            }
        }
    }

	// Render Objects
	//std::cout << "Rendering" << std::endl;
    SDL_RenderClear(renderer);
    renderer.render();
    SDL_RenderPresent(renderer);
}

void Game::setDrawColor(const SDL_Color& c) {
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
}
void Game::resetDrawColor() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}
void Game::setRenderTarget(SDL_Texture* tex) {
    if (SDL_SetRenderTarget(renderer, tex) != 0) {
        std::cout << "Unable to set render target" << std::endl;
    }
}
void Game::resetRenderTarget() {
    SDL_SetRenderTarget(renderer, NULL);
    resetDrawColor();
}

Rect Game::getAbsRect(const Rect& r) {
    return Rect(r.x + camera.x, r.y + camera.y, r.w, r.h);
}
