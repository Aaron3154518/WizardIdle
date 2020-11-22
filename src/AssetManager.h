#ifndef ASSET_MANAGER_h
#define ASSET_MANAGER_h

#include <sstream>
#include <string>
#include <map>
#include <sys/stat.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Rect.h"

// ARGB masks for creating surfaces and colors
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
constexpr Uint32 rmask = 0xff000000;
constexpr Uint32 gmask = 0x00ff0000;
constexpr Uint32 bmask = 0x0000ff00;
constexpr Uint32 amask = 0x000000ff;
#else
constexpr Uint32 rmask = 0x000000ff;
constexpr Uint32 gmask = 0x0000ff00;
constexpr Uint32 bmask = 0x00ff0000;
constexpr Uint32 amask = 0xff000000;
#endif

// Colors
constexpr SDL_Color WHITE   {255,255,255};
constexpr SDL_Color GRAY    {128,128,128};
constexpr SDL_Color BLACK   {0  ,0  ,0  };
constexpr SDL_Color RED     {255,0  ,0  };
constexpr SDL_Color ORANGE  {255,165,0  };
constexpr SDL_Color YELLOW  {255,255,0  };
constexpr SDL_Color GREEN   {0  ,255,0  };
constexpr SDL_Color CYAN    {0  ,255,255};
constexpr SDL_Color BLUE    {0  ,0  ,255};
constexpr SDL_Color MAGENTA {255,0  ,255};
constexpr SDL_Color PURPLE  {128,0  ,128};

static Uint32 toUint(SDL_Color c) {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    Uint8 arr[4] = {c.r, c.g, c.b, c.a};
    return *(Uint32*)arr;
#else
    Uint8 arr[4] = {c.a, c.b, c.g, c.r};
    return *(Uint32*)arr;
#endif
}

class AssetManager {
public:
    AssetManager();
    ~AssetManager();

    void loadAsset(std::string id, const char* fileName);
    void loadFont(std::string id, const char* fileName, int w, int h);

    SDL_Texture* getAsset(std::string id) const { return assets.find(id) == assets.end() ? assets.at("placeholder") : assets.at(id); }
    TTF_Font* getFont(std::string id) const { return fonts.find(id) == fonts.end() ? nullptr : fonts.at(id); }
    SDL_Texture* renderText(std::string fontID, const char* text, const SDL_Color& color) const;
    SDL_Texture* renderTextWrapped(std::string fontID, std::string text, const SDL_Color& color,
    	int w, Rect* rect, Uint32 bkgrnd = -1) const;
    void drawTexture(SDL_Texture* tex, Rect& destRect, Rect* boundary) const;
    void drawTexture(std::string id, Rect& destRect, Rect* boundary) const { drawTexture(getAsset(id), destRect, boundary); }
    void drawText(std::string fontID, const char* text, const SDL_Color& color, Rect destRect,
    	Rect* boundary) const;

private:
    std::map<std::string, SDL_Texture*> assets;
    std::map<std::string, TTF_Font*> fonts;
};

#endif /* Asset Manager */
