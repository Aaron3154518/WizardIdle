#include "Wizard.h"
#include "WizardContext.h"

// Wizard
Wizard::Wizard() : mFireballs(FireballHandler(CRYSTAL)) {}
void Wizard::init() {
    int w = Game::icon_w;
    mRect = Rect::getMinRect(Game::assets().getAsset(getImage()), w, w);
    mRect.setCenter(-Game::icon_w * 2, 0.);
}
void Wizard::update(Timestep ts) {
    power = basePower;
    power *= Wizards::crystal().mult_u.effect;

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
    for (Upgrade* u : mUpgrades) { u->update(ts); }
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

