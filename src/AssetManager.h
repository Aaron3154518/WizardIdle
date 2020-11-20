#ifndef ASSET_MANAGER_h
#define ASSET_MANAGER_h

#include <sstream>
#include <string>
#include <map>
#include <sys/stat.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Rect.h"
#include "Game.h"

// ARGB masks for creating surfaces and colors
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
constexpr Uint32 rmask = 0xff000000;
constexpr Uint32 gmask = 0x00ff0000;
constexpr Uint32 bmask = 0x0000ff00;
constexpr Uint32 amask = 0x000000ff;
constexpr Uint32 lgray = 0xaaaaaaff;
#else
constexpr Uint32 rmask = 0x000000ff;
constexpr Uint32 gmask = 0x0000ff00;
constexpr Uint32 bmask = 0x00ff0000;
constexpr Uint32 amask = 0xff000000;
constexpr Uint32 lgray = 0xffaaaaaa;
#endif

// Colors
#define WHITE   SDL_Color{255,255,255}
#define GRAY    SDL_Color{128,128,128}
#define BLACK   SDL_Color{0  ,0  ,0  }
#define RED     SDL_Color{255,0  ,0  }
#define ORANGE  SDL_Color{255,165,0  }
#define YELLOW  SDL_Color{255,255,0  }
#define GREEN   SDL_Color{0  ,255,0  }
#define CYAN    SDL_Color{0  ,255,255}
#define BLUE    SDL_Color{0  ,0  ,255}
#define MAGENTA SDL_Color{255,0  ,255}
#define PURPLE  SDL_Color{128,0  ,128}

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
		Uint32 bkgrnd, int w, Rect* rect) const;
	void drawTexture(SDL_Texture* tex, Rect& destRect, Rect* boundary) const;
	void drawTexture(std::string id, Rect& destRect, Rect* boundary) const { drawTexture(getAsset(id), destRect, boundary); }
	void drawText(std::string fontID, const char* text, const SDL_Color& color, Rect destRect,
		Rect* boundary) const;

private:
	Game game;
	std::map<std::string, SDL_Texture*> assets;
	std::map<std::string, TTF_Font*> fonts;
};

#endif /* Asset Manager */
