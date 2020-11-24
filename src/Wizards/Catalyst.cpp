#include "Catalyst.h"
#include "WizardContext.h"
#include "../Game.h"

// Catalyst
void Catalyst::init() {
    int w = Game::icon_w * 3 / 4;
    mRect = Rect::getMinRect(Game::assets().getAsset(getImage()), w, w);
    mRect.setCenter(-Game::icon_w * 2, 0.);

    pointsText.fontId = magicText.fontId = SMALL_FONT;
    pointsText.xMode = magicText.xMode = CENTER;
    pointsText.yMode = BOTRIGHT;
    magicText.yMode = TOPLEFT;
    pointsText.w = pointsText.h = 0;
    magicText.w = Game::icon_w;
}
void Catalyst::update(Timestep ts) {
    for (Upgrade* u : mUpgrades) { u->update(ts); }
}
void Catalyst::handleEvent(Event& e) {
    if (noDrag(*this, e)) { Wizards::upgradeManager().setSprite(CATALYST); }
}
void Catalyst::render() {
    Rect r = Game::getAbsRect(mRect);
    Game::assets().drawTexture(getImage(), r, NULL);
    std::stringstream ss;
    ss << points << "UP";
    pointsText.x = r.cX(); pointsText.y = r.y;
    pointsText.text = ss.str();
    Game::assets().drawText(pointsText, NULL);
    ss.str("");
    ss << magic << "/" << benchmark;
    magicText.x = r.cX(); magicText.y = r.y2();
    magicText.text = ss.str();
    Game::assets().drawTextWrapped(magicText, NULL);
}
void Catalyst::addMagic(Number add) {
    if (points < capacity) {
        magic += add;
        if (magic >= benchmark) {
            points += 1;
            if (points == capacity) {
                magic = benchmark;
            }
            benchmark ^= 1.2;
        }
    }
}
