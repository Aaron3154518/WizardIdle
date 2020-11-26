#include "Wizard.h"
#include "WizardData.h"
#include "../Game.h"

// Wizard
Wizard::Wizard() : mFireballs(FireballHandler({CRYSTAL, CATALYST})) {}
void Wizard::init() {
    int w = Game::get().icon_w;
    mRect = Rect::getMinRect(Game::get().assets.getAsset(getImage()), w, w);
    mRect.setCenter(-Game::get().icon_w * 2, 0.);

    target_u.toggleVisibility();
    mUpgrades = std::vector<Upgrade*> {&target_u};
    Sprite::init();
}
void Wizard::update(Timestep ts) {
    power = basePower;
    power *= Game::get().wizards.crystal.mult_u.effect;

    std::vector<Fireball> vec = mFireballs.update(ts);
    for (Fireball& f : vec) {
        std::stringstream ss;
        switch (f.mTarget) {
            case CRYSTAL:
                Game::get().wizards.crystal.magic += f.mData;
                ss << "+" << f.mData << "M";
                Game::get().wizards.crystal.addMessage(ss.str());
                break;
            case CATALYST:
                Game::get().wizards.catalyst.addMagic(f.mData);
                ss << "+" << f.mData << "M";
                Game::get().wizards.catalyst.addMessage(ss.str());
                break;
        }
    }
    vec.clear();
    if (mFireballs.ready()) {
        mFireballs.newFireball(mRect.cX(), mRect.cY(), power);
    }

    Sprite::update(ts);
}
void Wizard::handleEvent(Event& e) {
    if (drag(*this, e)) { Game::get().wizards.upgradeManager.select(WIZARD); }
//    Game::get().wizards.catalyst.mRect.setX2(mRect.x);
//    Game::get().wizards.catalyst.mRect.setCenterY(mRect.cY());
}
void Wizard::render() {
    Rect r = Game::get().getAbsRect(mRect);
    Game::get().assets.drawTexture(getImage(), r, NULL);
    mFireballs.render();

    Sprite::render();
}

// Target Upgrade
Wizard::TargetU::TargetU() :
    Upgrade(-1, "crystal", [&](){
            return "Currently: " + Game::get().wizards.getSprite(mTarget).getImage();
            }) {}
void Wizard::TargetU::levelUp() {
    int oldTarget = mTarget;
    mTarget = Game::get().wizards.wizard.mFireballs.nextTarget();
    if (oldTarget != mTarget) {
        mImg = Game::get().wizards.getSprite(mTarget).getImage();
        updateMe = true;
    }
}
