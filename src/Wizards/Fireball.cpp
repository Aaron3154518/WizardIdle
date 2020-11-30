#include "Fireball.h"
#include "WizardData.h"
#include "../Game.h"

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
    mRect = Rect::getMinRect(Game::get().assets.getAsset(mImg), mSize, mSize);
    mRect.setCenter(cX, cY);
}
void Fireball::setSize(int size) {
    mRect.resizeFactor((double)size / mSize, true);
    mSize = size;
}

// Fireball Handler
FireballHandler::FireballHandler(int spriteId, std::vector<int> targets) :
    Upgrade(-1, "", [&](){
            return "Currently: " + Game::get().wizards.getSprite(getTarget()).getImage();
            }), mSId(spriteId), mTargets(targets) {}
void FireballHandler::update(Timestep ts) {
    for (auto it = mFireballs.begin(); it != mFireballs.end();) {
        if (it->update(Game::get().wizards.getSprite(it->mTarget).mRect, ts)) {
            mCollided.push_back(*it);
            it = mFireballs.erase(it);
            continue;
        }
        ++it;
    }
    if (mTargets.size() != 0 &&
            Game::get().wizards.getSprite(getTarget()).mVisible) {
        mTimer -= ts.GetMilliseconds();
        if (mTimer <= 0) {
            mTimer = mDelay;
            Sprite& s = Game::get().wizards.getSprite(mSId);
            Fireball f(s.mRect.cX(), s.mRect.cY());
            f.mTarget = mTargets.at(mTIdx);
            f.mData = power;
            f.setSize((int)(Game::get().icon_w / 2));
            f.setImage(mFImg);
            mFireballs.push_back(f);
        }
    }
}
void FireballHandler::render() {
    for (Fireball& f : mFireballs) {
        Rect r = Game::get().getAbsRect(f.mRect);
        Game::get().assets.drawTexture(f.getImage(), r, NULL);
    }
}
void FireballHandler::levelUp() {
    if (mTargets.size() <= 1) { return; }
    int oldIdx = mTIdx;
    mTIdx = (mTIdx + 1) % mTargets.size();
    while (mTIdx != oldIdx && !Game::get().wizards.getSprite(getTarget()).mVisible) {
        mTIdx = (mTIdx + 1) % mTargets.size();
    }
    if (mTIdx != oldIdx) { updateImage(); }
}
std::vector<Fireball> FireballHandler::getCollided() {
    std::vector<Fireball> vecCopy = mCollided;
    mCollided.clear();
    return vecCopy;
}
void FireballHandler::setTarget(int target) {
    int i = 0;
    for (int t : mTargets) {
        if (t == target) { mTIdx = i; updateImage(); return; }
        ++i;
    }
    mTargets.push_back(target);
    mTIdx = mTargets.size() - 1;
    updateImage();
}
void FireballHandler::updateImage() {
    mImg = Game::get().wizards.getSprite(getTarget()).getImage();
    updateMe = true;
}
