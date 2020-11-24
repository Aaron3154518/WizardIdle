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

    mult_u.init();
    wizard_u.init();
    catalyst_u.init();
    mUpgrades = { &mult_u, &wizard_u };
}
void Crystal::update(Timestep ts) {
    for (Upgrade* u : mUpgrades) { u->update(ts); }
}
void Crystal::handleEvent(Event& e) {
    if (Sprite::noDrag(*this, e)) { Wizards::upgradeManager().setSprite(CRYSTAL); }
}
void Crystal::render() {
    Rect r = Game::getAbsRect(mRect);
    Game::assets().drawTexture(getImage(), r, NULL);
    std::stringstream ss;
    ss << magic << "M";
    magicText.x = r.cX(); magicText.y = r.y;
    magicText.text = ss.str();
    Game::assets().drawText(magicText, NULL);
}

// Mult Upgrade
void Crystal::MultU::update(Timestep ts) {
    effect = (Wizards::crystal().magic + 1).logBase(10) + 1;
}

// Wizard Upgrade
void Crystal::WizardU::levelUp() {
    ++mLevel;
    Wizards::wizard().mVisible = true;
    Wizards::crystal().mUpgrades.push_back(&Wizards::crystal().catalyst_u);
    Wizards::crystal().cost = Number(1, 2);
}

// Catalyst Upgrade
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


