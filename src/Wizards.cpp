#include "Wizards.h"
#include "WizardContext.h"

bool drag(Sprite& s, Event& e);
bool noDrag(Sprite& s, Event& e);

bool drag(Sprite& s, Event& e) {
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
bool noDrag(Sprite& s, Event& e) {
    if (e.left.clicked && SDL_PointInRect(&e.globalMouse, &s.mRect)) {
        e.handled = true;
        return true;
    }
    return false;
}

// Crystal
void Crystal::init() {
    int w = Game::icon_w * 2;
    mRect = Rect::getMinRect(Game::assets().getAsset(getImage()), w, w);
    mRect.setCenter(0., 0.);
    mVisible = true;

    wizard_u.init();
    catalyst_u.init();
    mUpgrades = { &wizard_u };

    magic = Number(1, 5);
}
void Crystal::update(Timestep ts) {
    power = (magic + 1).logBase(10) + 1;
}
void Crystal::handleEvent(Event& e) {
    if (noDrag(*this, e)) { Wizards::upgradeManager().setSprite(CRYSTAL); }
}
void Crystal::render() {
    Rect r = Game::getAbsRect(mRect);
    Game::assets().drawTexture(getImage(), r, NULL);
    std::stringstream ss;
    ss << magic << "M";
    Rect textR = Rect(0, 0, Game::icon_w * 3, Game::icon_w * 1.5);
    textR.setY2(r.y);
    textR.setCenterX(r.cX());
    Game::assets().drawText(LARGE_FONT, ss.str().c_str(), BLACK, textR, NULL);
}
// Crystal Upgrades
void Crystal::WizardU::levelUp() {
    ++mLevel;
    Wizards::wizard().mVisible = true;
    Wizards::crystal().mUpgrades.push_back(&Wizards::crystal().catalyst_u);
    Wizards::crystal().cost = Number(1, 5);
}
Crystal::CatalystU::CatalystU() :
    Upgrade([&]() {
            if (mLevel == 0) {
                std::stringstream ss;
                ss << Wizards::crystal().cost << "M";
                return ss.str().c_str();
            }
            return "Bought";
            }) {}
void Crystal::CatalystU::levelUp() {
    ++mLevel;
    Wizards::catalyst().mVisible = true;
    Wizards::crystal().magic -= Wizards::crystal().cost;
    Wizards::crystal().cost ^= 1.9;
}
bool Crystal::CatalystU::canBuy() {
    return mLevel == 0 && Wizards::crystal().cost <= Wizards::crystal().magic;
}

// Catalyst
void Catalyst::init() {
    int w = Game::icon_w * 3 / 4;
    mRect = Rect::getMinRect(Game::assets().getAsset(getImage()), w, w);
    mRect.setCenter(-Game::icon_w * 2, 0.);
}
void Catalyst::update(Timestep ts) {
    power = (magic ^ .3) + 1;
}
void Catalyst::handleEvent(Event& e) {
    if (noDrag(*this, e)) { Wizards::upgradeManager().setSprite(CATALYST); }
}
void Catalyst::render() {
    Rect r = Game::getAbsRect(mRect);
    Game::assets().drawTexture(getImage(), r, NULL);
}

// Wizard
Wizard::Wizard() : mFireballs(FireballHandler(CRYSTAL)) {}
void Wizard::init() {
    int w = Game::icon_w;
    mRect = Rect::getMinRect(Game::assets().getAsset(getImage()), w, w);
    mRect.setCenter(-Game::icon_w * 2, 0.);
}
void Wizard::update(Timestep ts) {
    power = basePower;
    power *= Wizards::crystal().power;
    power *= Wizards::catalyst().power;

    std::vector<Fireball> vec = mFireballs.update(ts);
    for (Fireball& f : vec) {
        switch (f.mTarget) {
            case CRYSTAL:
                Wizards::crystal().magic += f.mData;
                break;
        }
    }
    vec.clear();
    if (mFireballs.ready()) {
        mFireballs.newFireball(mRect.cX(), mRect.cY(), power);
    }
}
void Wizard::handleEvent(Event& e) {
    if (drag(*this, e)) { Wizards::upgradeManager().setSprite(WIZARD); }
    Wizards::catalyst().mRect.setX2(mRect.x);
    Wizards::catalyst().mRect.setCenterY(mRect.cY());
}
void Wizard::render() {
    Rect r = Game::getAbsRect(mRect);
    Game::assets().drawTexture(getImage(), r, NULL);
    mFireballs.render();
}

