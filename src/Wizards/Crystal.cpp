#include "Crystal.h"
#include "WizardData.h"
#include "../Game.h"

// Crystal
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

    mult_u.toggleVisibility();
    wizard_u.toggleVisibility();
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

// Wizard Upgrade
Crystal::WizardU::WizardU() :
    Upgrade(1, "wizard", [&]() { return mLevel == 0 ? "Free" : "Bought"; }) {}
void Crystal::WizardU::levelUp() {
    ++mLevel;
    Game::get().wizards.wizard.mVisible = true;
    Crystal& crystal = Game::get().wizards.crystal;
    crystal.catalyst_u.toggleVisibility();
    crystal.powerWizard_u.toggleVisibility();
    crystal.cost = Number(1, 2);
}

// Catalyst Upgrade
Crystal::CatalystU::CatalystU() :
    Upgrade(1, "catalyst", [&]() {
            if (mLevel == 0) {
                std::stringstream ss;
                ss << Game::get().wizards.crystal.cost << "M";
                return ss.str().c_str();
            }
            return "Bought";
            }) {}
void Crystal::CatalystU::levelUp() {
    ++mLevel;
    auto& wizards = Game::get().wizards;
    wizards.catalyst.mVisible = true;
    wizards.crystal.magic -= wizards.crystal.cost;
    if (!wizards.powerWizard.mVisible) { wizards.crystal.cost = Number(1, 4); }
    else {
        wizards.crystal.cost ^= 1.9;
        // Toggle idle/time wizard_u vis
    }
}
bool Crystal::CatalystU::canBuy() {
    return mLevel == 0 && Game::get().wizards.crystal.cost <= Game::get().wizards.crystal.magic;
}
// Power Wizard Upgrade
Crystal::PowerWizardU::PowerWizardU() :
    Upgrade(1, "powerWizard", [&]() {
            if (mLevel == 0) {
                std::stringstream ss;
                ss << Game::get().wizards.crystal.cost << "M";
                return ss.str().c_str();
            }
            return "Bought";
            }) {}
void Crystal::PowerWizardU::levelUp() {
    ++mLevel;
    auto& wizards = Game::get().wizards;
    wizards.powerWizard.mVisible = true;
    wizards.crystal.magic -= wizards.crystal.cost;
    if (!wizards.catalyst.mVisible) { wizards.crystal.cost = Number(1, 4); }
    else {
        wizards.crystal.cost ^= 1.9;
        // Toggle idle/time wizard_u vis
    }
}
bool Crystal::PowerWizardU::canBuy() {
    return mLevel == 0 && Game::get().wizards.crystal.cost <= Game::get().wizards.crystal.magic;
}

