#include "Sprite.h"

bool Sprite::drag(Sprite& s, Event& e) {
    if (e.left.clicked) {
        if (!s.mDragging && SDL_PointInRect(&e.globalMouse, &s.mRect)) {
            e.handled = true;
            return true;
        }
        s.mDragging = false;
    }
    else if (e.left.pressed && e.left.duration >= 250 &&
        SDL_PointInRect(&e.left.clickPosGlobal, &s.mRect)) {
        s.mDragging = true;
        e.handled = true;
    }

    if (s.mDragging) {
        s.mRect.setCenter(e.globalMouse.x, e.globalMouse.y);
        e.handled = true;
    }
    return false;
}
bool Sprite::noDrag(Sprite& s, Event& e) {
    if (e.left.clicked && SDL_PointInRect(&e.globalMouse, &s.mRect)) {
        e.handled = true;
        return true;
    }
    return false;
}


