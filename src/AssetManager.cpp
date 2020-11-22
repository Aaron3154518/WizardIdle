#include "AssetManager.h"
#include "Game.h"

AssetManager::AssetManager() {}
AssetManager::~AssetManager() {}

void AssetManager::loadAsset(std::string id, const char* fileName) {
    struct stat buffer;
    if (stat(fileName, &buffer) != 0) {
        std::cout << "Could not find " << fileName << std::endl;
        return;
    }
    else {
        std::cout << "Successfully loaded asset: " << id << std::endl;
    }
    SDL_Surface* tmpSurface = IMG_Load(fileName);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer(), tmpSurface);
    SDL_FreeSurface(tmpSurface);
    assets.emplace(id, tex);
}

void AssetManager::loadFont(std::string id, const char* fileName, int maxW, int maxH) {
    int minSize = 1, maxSize = 10;
    TTF_Font* font = TTF_OpenFont(fileName, maxSize);
    int w, h;
    TTF_SizeText(font, "|", &w, &h);
    while ((maxW != -1 && w < maxW) || (maxH != -1 && h < maxH)) {
        minSize = maxSize;
        maxSize *= 2;
        font = TTF_OpenFont(fileName, maxSize);
        TTF_SizeText(font, "|", &w, &h);
    }
    TTF_Font* font2;
    while (true) {
        int size = (int)((maxSize + minSize) / 2);
        font = TTF_OpenFont(fileName, size);
        TTF_SizeText(font, "|", &w, &h);
        if ((maxW != -1 && w < maxW) || (maxH != -1 && h < maxH)) {
            font2 = TTF_OpenFont(fileName, size + 1);
            TTF_SizeText(font2, "|", &w, &h);
            if ((maxW == -1 || w >= maxW) && (maxH == -1 || h >= maxH)) {
                fonts[id] = font2;
                return;
            }
            else {
                minSize = size + 1;
            }
        }
        else {
            font2 = TTF_OpenFont(fileName, size - 1);
            TTF_SizeText(font2, "|", &w, &h);
            if ((maxW == -1 || w >= maxW) && (maxH == -1 || h >= maxH)) {
                maxSize = size - 1;
            }
            else {
                fonts[id] = font2;
                return;
            }
        }
    }
}

SDL_Texture* AssetManager::renderText(std::string fontID, const char* text, const SDL_Color& color) const {
    TTF_Font* font = getFont(fontID);
    if (font == nullptr) {
        std::cout << "Font: " << fontID << " not loaded" << std::endl;
        return nullptr;
    }
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer(), surface);
    SDL_FreeSurface(surface);
    return texture;
}

SDL_Texture* AssetManager::renderTextWrapped(std::string fontID, std::string text, const SDL_Color& color,
    int w, Rect* rect, Uint32 bkgrnd) const {
    TTF_Font* font = getFont(fontID);
    if (font == nullptr) {
        std::cout << "Font: " << fontID << " not loaded" << std::endl;
        return nullptr;
    }

    std::vector<std::string> lines;
    std::stringstream line_ss, word_ss;
    int spaceW;
    TTF_SizeText(font, " ", &spaceW, NULL);
    int width = 0;
    for (char ch : text + '\n') {
        if (ch == ' ' || ch == '\n') {
            std::string word = word_ss.str();
            word_ss.str("");
            int wordW;
            TTF_SizeText(font, word.c_str(), &wordW, NULL);
            if (width + wordW < w) {
                if (width != 0) { line_ss << ' '; }
                line_ss << word;
                width += wordW + spaceW;
            }
            else {
                lines.push_back(line_ss.str());
                line_ss.str("");
                line_ss << word;
                width = wordW + spaceW;
            }
            if (ch == '\n') {
                lines.push_back(line_ss.str());
                line_ss.str("");
                width = 0;
            }
        }
        else {
            word_ss << ch;
        }
    }

    int lineH;
    TTF_SizeText(font, "|", NULL, &lineH);
    *rect = Rect(0, 0, w, (int)(lineH * lines.size()));
    SDL_Surface* surf = SDL_CreateRGBSurface(0, rect->w, rect->h, 32, rmask, gmask, bmask, amask);
    if (bkgrnd != -1) { SDL_FillRect(surf, NULL, bkgrnd); }
    int x = w / 2, y = lineH / 2;
    for (std::string line : lines) {
        if (line == "") { y += lineH; continue; }
        SDL_Surface* lineSurf = TTF_RenderText_Blended(font, line.c_str(), color);
        if (lineSurf != nullptr) {
            Rect lineRect = Rect(0, 0, lineSurf->w, lineSurf->h);
            lineRect.setCenter(x, y);
            SDL_BlitSurface(lineSurf, NULL, surf, &lineRect);
            SDL_FreeSurface(lineSurf);
        }
        else {
            std::cout << "line '" << line << "' produced a null surface" << std::endl;
        }
        y += lineH;
    }
    lines.clear();
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer(), surf);
    SDL_FreeSurface(surf);
    return tex;
}

void AssetManager::drawTexture(SDL_Texture* tex, Rect& destRect, Rect* boundary) const {
    if (tex == nullptr) {
        std::cout << "Invalid Texture" << std::endl;
        return;
    }
    Rect screen = Game::screen();
    if (boundary == nullptr) {
        boundary = &screen;
    }
    else {
        SDL_IntersectRect(boundary, &screen, boundary);
    }

    double leftFrac = fmax(boundary->x - destRect.x, 0) / destRect.w;
    double topFrac = fmax(boundary->y - destRect.y, 0) / destRect.h;
    double rightFrac = fmax(destRect.x2() - boundary->x2(), 0) / destRect.w;
    double botFrac = fmax(destRect.y2() - boundary->y2(), 0) / destRect.h;
    // Make sure the rect is actually in the boundary
    if (leftFrac + rightFrac >= 1 || topFrac + botFrac >= 1) {
#ifdef RENDER_DEBUG
        std::cout << "Rect " << destRect << " was out side the boundary " << *boundary << std::endl;
#endif
        return;
    }

    Rect drawRect = Rect(destRect.x + (int)(destRect.w * leftFrac), destRect.y + (int)(destRect.h * topFrac),
        (int)(destRect.w * (1 - leftFrac - rightFrac)), (int)(destRect.h * (1 - topFrac - botFrac)));
    int w, h;
    if (SDL_QueryTexture(tex, NULL, NULL, &w, &h) != 0) {
        std::cout << "Unable to query texture size: " 
            << SDL_GetError() << std::endl;
        return;
    }
    Rect texRect = Rect((int)(w * leftFrac), (int)(h * topFrac),
        (int)(w * (1 - leftFrac - rightFrac)), (int)(h * (1 - topFrac - botFrac)));
    // Make sure at least one pixel will be drawn
    if (drawRect.w == 0 || drawRect.h == 0 || texRect.w == 0 || texRect.h == 0) {
#ifdef RENDER_DEBUG
        std::cout << "Can't draw from " << texRect << " to " << drawRect << std::endl;
#endif
        return;
    }

    SDL_RenderCopy(Game::renderer(), tex, &texRect, &drawRect);
}

void AssetManager::drawText(std::string fontID, const char* text, const SDL_Color& color,
    Rect destRect, Rect* boundary) const { 
    SDL_Texture* tex = renderText(fontID, text, color);
    if (tex != nullptr) {
        double cX = destRect.cX(), cY = destRect.cY();
        destRect = Rect::getMinRect(tex, destRect.w, destRect.h);
        destRect.setCenter(cX, cY);
        drawTexture(tex, destRect, boundary);
        SDL_DestroyTexture(tex);
    }
}
