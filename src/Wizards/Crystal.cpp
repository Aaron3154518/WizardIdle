#include "Crystal.h"
#include "WizardContext.h"

// Crystal
void Crystal::init() {
    int w = Game::icon_w * 2;
    mRect = Rect::getMinRect(Game::assets().getAsset(getImage()), w, w);
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
    if (Sprite::noDrag(*this, e)) { Wizards::upgradeManager().select(CRYSTAL); }
}
void Crystal::render() {
    Rect r = Game::getAbsRect(mRect);
    Game::assets().drawTexture(getImage(), r, NULL);
    std::stringstream ss;
    ss << magic << "M";
    magicText.x = r.cX(); magicText.y = r.y;
    magicText.text = ss.str();
    Game::assets().drawText(magicText, NULL);

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
    effect = ((Wizards::crystal().magic + 1).logBase(10) + 1) ^ 1.5;
}

// Wizard Upgrade
Crystal::WizardU::WizardU() :
    Upgrade(1, "wizard", [&]() { return mLevel == 0 ? "Free" : "Bought"; }) {}
void Crystal::WizardU::levelUp() {
    ++mLevel;
    Wizards::wizard().mVisible = true;
    Crystal& crystal = Wizards::crystal();
    crystal.catalyst_u.toggleVisibility();
    crystal.powerWizard_u.toggleVisibility();
    crystal.cost = Number(1, 2);
}

// Catalyst Upgrade
Crystal::CatalystU::CatalystU() :
    Upgrade(1, "catalyst", [&]() {
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
// Power Wizard Upgrade
Crystal::PowerWizardU::PowerWizardU() :
    Upgrade(1, "powerWizard", [&]() {
            if (mLevel == 0) {
                std::stringstream ss;
                ss << Wizards::crystal().cost << "M";
                return ss.str().c_str();
            }
            return "Bought";
            }) {}
void Crystal::PowerWizardU::levelUp() {
    ++mLevel;
    Wizards::powerWizard().mVisible = true;
    Wizards::crystal().magic -= Wizards::crystal().cost;
    Wizards::crystal().cost ^= 1.9;
}
bool Crystal::PowerWizardU::canBuy() {
    return mLevel == 0 && Wizards::crystal().cost <= Wizards::crystal().magic;
}

