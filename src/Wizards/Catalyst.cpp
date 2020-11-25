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

    mUpgrades = std::vector<Upgrade*> {};
    Sprite::init();
}
void Catalyst::handleEvent(Event& e) {
    if (noDrag(*this, e)) { Wizards::upgradeManager().select(CATALYST); }
}
void Catalyst::render() {
    Rect r = Game::getAbsRect(mRect);
    Game::assets().drawTexture(getImage(), r, NULL);
    r.h = (int)(Game::text_h * .75);
    r.setY2(r.y + 1);
    if (points != capacity) {
        Game::assets().drawProgressBar(magic, goal, r, BLUE, LGRAY);
    } else {
        Game::assets().drawProgressBar(1, 1, r, YELLOW, LGRAY);
    }
    std::stringstream ss;
    ss << points << "UP";
    pointsText.x = r.cX(); pointsText.y = r.y;
    pointsText.text = ss.str();
    Game::assets().drawText(pointsText, NULL);

    Sprite::render();
}
void Catalyst::addMagic(Number add) {
    if (points < capacity) {
        magic += add;
        Number oldPoints = points;
        while (magic >= goal) {
            points += 1;
            if (points == capacity) { magic = 0; break; }
            else { magic -= goal; }
            goal ^= 1.2;
        }
        if (points != oldPoints) {
            std::stringstream ss;
            ss << "+" << (points - oldPoints) << "UP";
            addMessage(ss.str(), BLUE);
        }
    }
}
