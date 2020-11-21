#include "WizardContext.h"
#include "Game.h"
#include "AssetManager.h"

void drag(Sprite& s, Event& e);
void noDrag(Sprite& s, Event& e);

void drag(Sprite& s, Event& e) {
    if (e.left.clicked) {
        if (!s.mDragging && SDL_PointInRect(&e.globalMouse, &s.mRect)) {
            // Set upgrade list
            e.handled = true;
        }
        s.mDragging = false;
    }
    else if (e.left.pressed && e.left.duration >= 250 &&
        SDL_PointInRect(&e.left.clickPos, &s.mRect)) {
        s.mDragging = true;
        e.handled = true;
    }

    if (s.mDragging) {
        s.mRect.setCenter(e.globalMouse.x, e.globalMouse.y);
        e.handled = true;
    }
}
void noDrag(Sprite& s, Event& e) {
    if (e.left.clicked && SDL_PointInRect(&e.globalMouse, &s.mRect)) {
        // Set upgrade list
        e.handled = true;
    }
}

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
    mRect = Rect::getMinRect(Game().assets().getAsset(mImg), mSize, mSize);
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
    Game g;
    for (Fireball& f : mFireballs) {
        Rect r = g.getAbsRect(f.mRect);
        g.assets().drawTexture(f.getImage(), r, NULL);
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

// Crystal
void Crystal::update(WizardContext& wc, Timestep ts) {
	power = (magic + 1).logBase(10) + 1;
}
void Crystal::handleEvent(WizardContext& wc, Event& e) {
	noDrag(*this, e);
}
void Crystal::render(WizardContext& wc) {
    Game g;
    Rect r = g.getAbsRect(mRect);
	g.assets().drawTexture("crystal", r, NULL);
}

// Catalyst
void Catalyst::update(WizardContext& wc, Timestep ts) {
	power = (magic ^ .3) + 1;
}
void Catalyst::handleEvent(WizardContext& wc, Event& e) {
	noDrag(*this, e);
}
void Catalyst::render(WizardContext& wc) {
    Game g;
    Rect r = g.getAbsRect(mRect);
    g.assets().drawTexture("catalyst", r, NULL);
}

// Wizard
void Wizard::update(WizardContext& wc, Timestep ts) {
    std::vector<Fireball> vec = mFireballs.update(wc, ts);
    for (Fireball& f : vec) {
        switch (f.mTarget) {
            case CRYSTAL:
                wc.crystal.magic += f.mData;
                break;
        }
    }
    vec.clear();
    if (mFireballs.ready()) {
        mFireballs.newFireball(mRect.cX(), mRect.cY(), power);
    }
}
void Wizard::handleEvent(WizardContext& wc, Event& e) {
	drag(*this, e);
    wc.catalyst.mRect.setX2(mRect.x);
    wc.catalyst.mRect.setCenterY(mRect.cY());
}
void Wizard::render(WizardContext& wc) {
    Game g;
    Rect r = g.getAbsRect(mRect);
    g.assets().drawTexture("wizard", r, NULL);
    mFireballs.render();
}

void WizardContext::init() {
    AssetManager am = Game().assets();
    int w = Game::icon_w * 3 / 2;
    crystal.mRect = Rect::getMinRect(am.getAsset("crystal"), w, w);
    crystal.mRect.setCenter(0, 0);
    w = Game::icon_w / 2;
    catalyst.mRect = Rect::getMinRect(am.getAsset("catalyst"), w, w);
    w = Game::icon_w;
    wizard.mRect = Rect::getMinRect(am.getAsset("wizard"), w, w);
    wizard.mRect.setCenter(0, 0);

    crystal.mVisible = true;
    wizard.mVisible = true;
}
void WizardContext::update(Timestep ts) {
    // Custom update order? (or just custom update no function calls)
    for (Sprite* s : mSprites) {
        if (s->mVisible) { s->update(*this, ts); }
    }
}
void WizardContext::handleEvent(Event& e) {
    for (auto it = mSprites.end(); it != mSprites.begin();) {
        --it;
        if ((*it)->mVisible) { (*it)->handleEvent(*this, e); }
    }
}
void WizardContext::render() {
    for (Sprite* s : mSprites) {
        if (s->mVisible) { s->render(*this); }
    }
}
Sprite& WizardContext::getSprite(int id) {
    switch (id) {
        case CATALYST: return catalyst; break;
        case WIZARD: return wizard; break;
        default: return crystal; break;
    }
    return crystal;
}
