#include "Fireball.h"
#include "Wizards.h"
#include "WizardContext.h"

// Fireball
bool Fireball::update(Rect& target, Timestep ts) {
    double dx = target.cX() - x, dy = target.cY() - y;
    double mag = sqrt(dx*dx + dy*dy);
    if (mag <= 10) { return true; }
    setPos(x + dx * maxV / mag, y + dy * maxV / mag);
    return false;
}
void Fireball::setPos(double nX, double nY) {
    x = nX;
    y = nY;
    mRect.setCenter(x, y);
}
void Fireball::setImage(std::string img) {
    mImg = img;
    double cX = mRect.cX(), cY = mRect.cY();
    mRect = Rect::getMinRect(Game::assets().getAsset(mImg), mSize, mSize);
    mRect.setCenter(cX, cY);
}
void Fireball::setSize(int size) {
    mRect.resizeFactor((double)size / mSize, true);
    mSize = size;
}
 
std::vector<Fireball> FireballHandler::update(WizardContext& wc, Timestep ts) {
    mTimer -= ts.GetMilliseconds();
    std::vector<Fireball> vec;
    for (auto it = mFireballs.begin(); it != mFireballs.end(); ++it) {
        if (it->update(wc.getSprite(it->mTarget).mRect, ts)) {
            vec.push_back(*it);
            it = mFireballs.erase(it);
            if (it == mFireballs.end()) { break; }
        }
    }
    return vec;
}
void FireballHandler::render() {
    for (Fireball& f : mFireballs) {
        Rect r = Game::getAbsRect(f.mRect);
        Game::assets().drawTexture(f.getImage(), r, NULL);
    }
}
void FireballHandler::newFireball(double x, double y, Number data) {
    mTimer = mDelay;
    Fireball f(x, y);
    f.mTarget = mTarget;
    f.mData = data;
    f.setSize((int)(Game::icon_w / 2));
    f.setImage(mImg);
    mFireballs.push_back(f);
}

