#include "Wizard.h"
#include "WizardContext.h"

// Wizard
Wizard::Wizard() : mFireballs(FireballHandler({CRYSTAL, CATALYST})) {}
void Wizard::init() {
    int w = Game::icon_w;
    mRect = Rect::getMinRect(Game::assets().getAsset(getImage()), w, w);
    mRect.setCenter(-Game::icon_w * 2, 0.);

    target_u.init();
    mUpgrades = { &target_u };

    Sprite::init();
}
void Wizard::update(Timestep ts) {
    power = basePower;
    power *= Wizards::crystal().mult_u.effect;

    std::vector<Fireball> vec = mFireballs.update(ts);
    for (Fireball& f : vec) {
        std::stringstream ss;
        switch (f.mTarget) {
            case CRYSTAL:
                Wizards::crystal().magic += f.mData;
                ss << "+" << f.mData << "M";
                Wizards::crystal().addMessage(ss.str());
                break;
            case CATALYST:
                Wizards::catalyst().addMagic(f.mData);
                ss << "+" << f.mData << "M";
                Wizards::catalyst().addMessage(ss.str());
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
    if (drag(*this, e)) { Wizards::upgradeManager().setSprite(WIZARD); }
//    Wizards::catalyst().mRect.setX2(mRect.x);
//    Wizards::catalyst().mRect.setCenterY(mRect.cY());
}
void Wizard::render() {
    Rect r = Game::getAbsRect(mRect);
    Game::assets().drawTexture(getImage(), r, NULL);
    mFireballs.render();

    Sprite::render();
}

// Target Upgrade
Wizard::TargetU::TargetU() :
    Upgrade([&](){
            return "Currently: " + Wizards::getSprite(mTarget).getImage();
            }) {}
void Wizard::TargetU::levelUp() {
    int oldTarget = mTarget;
    mTarget = Wizards::wizard().mFireballs.nextTarget();
    if (oldTarget != mTarget) {
        mImg = Wizards::getSprite(mTarget).getImage();
        updateMe = true;
    }
}
