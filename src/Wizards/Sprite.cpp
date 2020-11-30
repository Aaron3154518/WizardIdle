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
    mUpgrades.init();
}
void Sprite::update(Timestep ts) {
    double lastY = 0.;
    for (auto it = mMessages.begin(); it != mMessages.end();) {
        if (!(*it)->permanent()) {
            (*it)->mTimer -= ts.GetMilliseconds();
            if ((*it)->mTimer <= 0) { (*it)->mActive = false; }
        }
        if (!(*it)->mActive) {
            it = mMessages.erase(it);
            continue;
        }
        double diff = (*it)->mY - lastY;
        if (diff != 0.) {
            int sign = diff < 0 ? -1 : 1;
            double dy = std::max((double)Game::get().text_h, std::abs(diff));
            dy = std::min(std::abs(diff), dy * ts.GetSeconds()) * sign;
            (*it)->mY -= dy;
        }
        lastY = (*it)->bottom();
        ++it;
    }

    mUpgrades.update(ts);
}
void Sprite::render() {
    int y = (int)(mRect.y2() + .5);
    double cX = mRect.cX();
    for (auto& m : mMessages) {
        Rect r = m->mRect;
        r.y = y + m->mY;
        r.setCenterX(cX);
        r = Game::get().getAbsRect(r);
        Game::get().assets.drawTexture(m->getTexture(), r, NULL);
    }
}
MessagePtr Sprite::newMessage(std::string text, SDL_Color color, int ms) {
    double y = 0.;
    if (!mMessages.empty()) {
        y = mMessages.back()->bottom();
    }
    mMessages.push_back(std::make_shared<Message>(text, color, ms));
    mMessages.back()->mY = y;
    return mMessages.back();
}

// Message
Message::Message(std::string text, SDL_Color color, int ms) : mTimer(ms) {
    mText.fontId = SMALL_FONT;
    mText.w = Game::get().icon_w * 6;
    mText.text = text;
    mText.color = color;
    redraw();
}
Message::Message(const Message& m) : mActive(m.mActive), mTimer(m.mTimer), 
    mY(m.mY), mRect(m.mRect), mText(m.mText) {
    redraw();
}
Message& Message::operator=(const Message& m) {
    mActive = m.mActive;
    mTimer = m.mTimer;
    mY = m.mY;
    mRect = m.mRect;
    mText = m.mText;
    redraw();
    return *this;
}
void Message::redraw() {
    destroy();
    mTex = Game::get().assets.renderTextWrapped(mText, mRect);
}
void Message::destroy() {
    if(mTex != nullptr) {
        SDL_DestroyTexture(mTex);
    }
}
