#include "Wizard.h"
#include "WizardData.h"
#include "../Game.h"

// Wizard
const std::vector<int> Wizard::TARGETS = {CRYSTAL, CATALYST};

Wizard::Wizard() : Sprite(WIZARD, "Wizard shoots fireballs at the crystal, "
        "increasing its magic (M)"), mFireballs{WIZARD, TARGETS} {}
void Wizard::init() {
    int w = Game::get().icon_w;
    mRect = Rect::getMinRect(Game::get().assets.getAsset(getImage()), w, w);
    mRect.setCenter(-Game::get().icon_w * 2, 0.);

    mFireballs.setVisible(true);
    mUpgrades = std::vector<Upgrade*> { &mFireballs };
    Sprite::init();
}
void Wizard::update(Timestep ts) {
    auto& wizards = Game::get().wizards;
    power = basePower;
    power *= wizards.crystal.mult_u.effect;
    power *= wizards.powerWizard.getMulti(mId);
    power *= wizards.catalyst.multi_u.getMulti(mId);

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
    drag(*this, e);
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

