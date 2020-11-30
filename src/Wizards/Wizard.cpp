#include "Wizard.h"
#include "WizardData.h"
#include "../Game.h"

// Wizard
const std::vector<int> Wizard::TARGETS = {CRYSTAL, CATALYST};

Wizard::Wizard(int id) : Sprite(id), mFireballs{id, TARGETS} {}
void Wizard::init() {
    int w = Game::get().icon_w;
    mRect = Rect::getMinRect(Game::get().assets.getAsset(getImage()), w, w);
    mRect.setCenter(-Game::get().icon_w * 2, 0.);

    mFireballs.toggleVisibility();
    mUpgrades = std::vector<Upgrade*> { &mFireballs };
    Sprite::init();
}
void Wizard::update(Timestep ts) {
    power = basePower;
    power *= Game::get().wizards.crystal.mult_u.effect;
    power *= Game::get().wizards.powerWizard.getMulti(mId);

    mFireballs.power = power;
//    std::vector<Fireball> vec = mFireballs.update(ts);
    for (Fireball& f : mFireballs.getCollided()) {
        std::stringstream ss;
        switch (f.mTarget) {
            case CRYSTAL:
                Game::get().wizards.crystal.magic += f.mData;
                ss << "+" << f.mData << "M";
                Game::get().wizards.crystal.newMessage(ss.str());
                break;
            case CATALYST:
                Game::get().wizards.catalyst.addMagic(f.mData);
                ss << "+" << f.mData << "M";
                Game::get().wizards.catalyst.newMessage(ss.str());
                break;
        }
    }
//    vec.clear();

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

