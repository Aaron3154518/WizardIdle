#include "RenderObject.h"

void RenderObject::setTexture(SDL_Texture* tex) {
	if (mTex != nullptr) { SDL_DestroyTexture(mTex); }
	mTex = tex;
}

void RenderObject::render(SDL_Renderer* renderer, Rect* boundary) {
    if (mTex == nullptr) {
        std::cout << "Invalid Texture" << std::endl;
        return;
    }
    Rect screen = Game::get().screen;
    if (boundary == nullptr) {
        boundary = &screen;
    }
    else {
        SDL_IntersectRect(boundary, &screen, boundary);
    }

    double leftFrac = fmax(boundary->x - mRect.x, 0) / mRect.w;
    double topFrac = fmax(boundary->y - mRect.y, 0) / mRect.h;
    double rightFrac = fmax(mRect.x2() - boundary->x2(), 0) / mRect.w;
    double botFrac = fmax(mRect.y2() - boundary->y2(), 0) / mRect.h;
    // Make sure the rect is actually in the boundary
    if (leftFrac + rightFrac >= 1 || topFrac + botFrac >= 1) {
#ifdef RENDER_DEBUG
        std::cout << "Rect " << mRect << " was out side the boundary " << *boundary << std::endl;
#endif
        return;
    }

    Rect drawRect = Rect(mRect.x + (int)(mRect.w * leftFrac), mRect.y + (int)(mRect.h * topFrac),
        (int)(mRect.w * (1 - leftFrac - rightFrac)), (int)(mRect.h * (1 - topFrac - botFrac)));
    int w, h;
    if (SDL_QueryTexture(mTex, NULL, NULL, &w, &h) != 0) {
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

    SDL_RenderCopy(renderer, mTex, &texRect, &drawRect);
}

void RenderObject::clean() {
	if (mTex != nullptr) { SDL_DestroyTexture(mTex); }
}

