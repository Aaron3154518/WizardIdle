#include "Sprite.h"
#include "../Game.h"

bool Sprite::drag(Sprite& s, Event& e) {
    if (e.left.clicked) {
        if (!s.mDragging && SDL_PointInRect(&e.globalMouse, &s.mRect)) {
            e.handled = true;
            return true;
        }
        if (s.mDragging) {
            s.mDragging = false;
            e.handled = true;
        }
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

void Sprite::init() {
    mText.fontId = SMALL_FONT;
    mText.w = Game::icon_w * 6;
}

void Sprite::update(Timestep ts) {
    if (mMessageY != 0) {
        double dy = std::max(mMessageY, (double)Game::text_h) / 20.;
        mMessageY -= dy;
        if (mMessageY < 0) { mMessageY = 0; }
    }
    bool top = true;
    for (auto it = mMessages.begin(); it != mMessages.end(); ++it) {
        it->timer -= ts.GetMilliseconds();
        if (top && it->timer <= 0) {
            mMessageY += it->rect.h;
            it->destroy();
            it = mMessages.erase(it);
            if (it == mMessages.end()) { break; }
        } else { top = false; }
    }

    for (Upgrade* u : mUpgrades) { u->update(ts); }
}

void Sprite::render() {
    int y = (int)(mRect.y2() + mMessageY + .5);
    double cX = mRect.cX();
    for (Message m : mMessages) { 
        m.rect.y = y;
        m.rect.setCenterX(cX);
        m.rect = Game::getAbsRect(m.rect);
        Game::assets().drawTexture(m.tex, m.rect, NULL);
        y += m.rect.h;
    }
}

void Sprite::addMessage(std::string text, SDL_Color color) {
    if (text == "") { return; }
    mText.text = text; mText.color = color;
    mMessages.push_back(Message()); // Push back now so we don't copy m.tex
    Message& m = mMessages.back();
    m.tex = Game::assets().renderTextWrapped(mText, m.rect);

}
