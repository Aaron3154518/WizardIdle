#include "Wizards.h"
#include "WizardContext.h"

bool drag(Sprite& s, Event& e);
bool noDrag(Sprite& s, Event& e);

bool drag(Sprite& s, Event& e) {
    if (e.left.clicked) {
        s.mDragging = false;
        if (!s.mDragging && SDL_PointInRect(&e.globalMouse, &s.mRect)) {
            e.handled = true;
            return true;
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
    mRect = Rect::getMinRect(Game::assets().getAsset("crystal"), w, w);
    mRect.setCenter(0., 0.);
    mVisible = true;

    wizard_u.init();
    mUpgrades = { &wizard_u };
}
void Crystal::update(WizardContext& wc, Timestep ts) {
    power = (magic + 1).logBase(10) + 1;
}
void Crystal::handleEvent(WizardContext& wc, Event& e) {
    if (noDrag(*this, e)) { wc.upgradeManager.setUpgrades(&mUpgrades); }
}
void Crystal::render(WizardContext& wc) {
    Rect r = Game::getAbsRect(mRect);
    Game::assets().drawTexture("crystal", r, NULL);
    std::stringstream ss;
    ss << magic << "M";
    Rect textR = Rect(0, 0, Game::icon_w * 3, Game::icon_w * 1.5);
    textR.setY2(r.y);
    textR.setCenterX(r.cX());
    Game::assets().drawText(LARGE_FONT, ss.str().c_str(), BLACK, textR, NULL);
}
void Crystal::WizardU::levelUp(WizardContext& wc) {
    ++mLevel;
    bought = true;
    wc.wizard.mVisible = true;
}

// Catalyst
void Catalyst::init() {
    int w = Game::icon_w * 3 / 4;
    mRect = Rect::getMinRect(Game::assets().getAsset("catalyst"), w, w);
    mRect.setCenter(-Game::icon_w * 2, 0.);
}
void Catalyst::update(WizardContext& wc, Timestep ts) {
    power = (magic ^ .3) + 1;
}
void Catalyst::handleEvent(WizardContext& wc, Event& e) {
    if (noDrag(*this, e)) { wc.upgradeManager.setUpgrades(&mUpgrades); }
}
void Catalyst::render(WizardContext& wc) {
    Rect r = Game::getAbsRect(mRect);
    Game::assets().drawTexture("catalyst", r, NULL);
}

// Wizard
Wizard::Wizard() : mFireballs(FireballHandler(CRYSTAL)) {}
void Wizard::init() {
    int w = Game::icon_w;
    mRect = Rect::getMinRect(Game::assets().getAsset("wizard"), w, w);
    mRect.setCenter(-Game::icon_w * 2, 0.);
}
void Wizard::update(WizardContext& wc, Timestep ts) {
    power = basePower;
    power *= wc.crystal.power;

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
    if (drag(*this, e)) { wc.upgradeManager.setUpgrades(&mUpgrades); }
    wc.catalyst.mRect.setX2(mRect.x);
    wc.catalyst.mRect.setCenterY(mRect.cY());
}
void Wizard::render(WizardContext& wc) {
    Rect r = Game::getAbsRect(mRect);
    Game::assets().drawTexture("wizard", r, NULL);
    mFireballs.render();
}

