#include "Crystal.h"
#include "WizardData.h"
#include "../Game.h"

// Crystal
Crystal::Crystal() : Sprite(CRYSTAL, "Crystal is evil") {}
void Crystal::init() {
    int w = Game::get().icon_w * 2;
    mRect = Rect::getMinRect(Game::get().assets.getAsset(getImage()), w, w);
    mRect.setCenter(0., 0.);
    mVisible = true;

    magicText.fontId = LARGE_FONT;
    magicText.color = PURPLE;
    magicText.xMode = CENTER;
    magicText.yMode = BOTRIGHT;
    magicText.w = magicText.h = 0;

    mult_u.setVisible(true);
    wizard_u.setVisible(true);
    mUpgrades = std::vector<Upgrade*> {&mult_u, &wizard_u, &catalyst_u, &powerWizard_u};
    Sprite::init();
}
void Crystal::handleEvent(Event& e) {
    noDrag(*this, e);
}
void Crystal::render() {
    Rect r = Game::get().getAbsRect(mRect);
    Game::get().assets.drawTexture(getImage(), r, NULL);
    std::stringstream ss;
    ss << magic << "M";
    magicText.x = r.cX(); magicText.y = r.y;
    magicText.text = ss.str();
    Game::get().assets.drawText(magicText, NULL);

    Sprite::render();
}

// Mult Upgrade
Crystal::MultU::MultU() :
    Upgrade(0, "crystal", [&]() {
            std::stringstream ss;
            ss << effect << "x";
            return ss.str();
            }) {}
void Crystal::MultU::update(Timestep ts) {
    effect = (max(Game::get().wizards.crystal.magic, 10)).logBase(10) ^ 1.8;
}

// Wizard Unlock Upgrade
Crystal::UnlockU::UnlockU(int id) : mId(id) {
    mMaxLevel = 1;
    if (id == WIZARD) {
        mGetInfo = [&]() { return mLevel == 0 ? "Free" : "Bought"; };
    } else {
        mGetInfo = [&]() { return mLevel == 0 ?
            Game::get().wizards.crystal.cost.toString() + "M" : "Bought"; };
    }
}
void Crystal::UnlockU::init() {
    mImg = Game::get().wizards.getSprite(mId).getImage(); 
    setDescription("Unlock " + mImg + "\n" + Game::get().wizards.getSprite(mId).getDescription());
}
void Crystal::UnlockU::levelUp() {
    ++mLevel;
    auto& wizards = Game::get().wizards;
    wizards.getSprite(mId).setVisible(true);
    setVisible(false);
    Crystal& crystal = wizards.crystal;
    crystal.magic -= crystal.cost;
    switch(mId) {
        case WIZARD:
            crystal.catalyst_u.setVisible(true);
            crystal.powerWizard_u.setVisible(true);
            crystal.cost = Number(50);
            break;
        case CATALYST:
        case POWER_WIZARD:
            if (wizards.powerWizard.visible() && wizards.catalyst.visible()) {
                wizards.crystal.cost ^= 1.9;
                // Toggle idle/time wizard_u vis
            } else { wizards.crystal.cost = Number(1, 4); }
            break;
        default:
            break;
    }
}
bool Crystal::UnlockU::canBuy() {
    return mLevel == 0 && Game::get().wizards.crystal.cost <= Game::get().wizards.crystal.magic;
}
