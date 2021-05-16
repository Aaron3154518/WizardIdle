#include "Catalyst.h"
#include "WizardData.h"
#include "../Game.h"

// Catalyst
Catalyst::Catalyst() : Sprite(CATALYST, "The wizard can shoot fireballs into the catalyst "
        "to fill it up and gain Upgrade Points (UP). Use UP to power up your wizards") {}
void Catalyst::init() {
    int w = Game::get().icon_w * 3 / 4;
    mRect = Rect::getMinRect(Game::get().assets.getAsset(getImage()), w, w);
    mRect.setCenter(-Game::get().icon_w * 2, 0.);

    pointsText.fontId = magicText.fontId = SMALL_FONT;
    pointsText.xMode = magicText.xMode = CENTER;
    pointsText.yMode = BOTRIGHT;
    magicText.yMode = TOPLEFT;
    pointsText.w = pointsText.h = 0;
    magicText.w = Game::get().icon_w;

    multi_u.setVisible(true);
    mUpgrades = std::vector<Upgrade*> { &multi_u };
    Sprite::init();
}
void Catalyst::handleEvent(Event& e) {
    noDrag(*this, e);
}
void Catalyst::render() {
    Rect r = Game::get().getAbsRect(mRect);
    Game::get().assets.drawTexture(getImage(), r, NULL);
    r.h = (int)(Game::get().text_h * .75);
    r.setY2(r.y + 1);
    if (points != capacity) {
        Game::get().assets.drawProgressBar(magic, goal, r, BLUE, LGRAY);
    } else {
        Game::get().assets.drawProgressBar(1, 1, r, YELLOW, LGRAY);
    }
    std::stringstream ss;
    ss << points << "UP";
    pointsText.x = r.cX(); pointsText.y = r.y;
    pointsText.text = ss.str();
    Game::get().assets.drawText(pointsText, NULL);

    Sprite::render();
}
void Catalyst::addMagic(Number add) {
    if (points < capacity) {
        magic += add;
        if (magic >= goal) {
            Number gain = goalFactor;
            // m/g = (gF^n - 1) / (gF - 1)
            // n = log(m/g * (gF - 1) + 1) / log(gF)
            gain.subtract(1).multiply(magic).divide(goal).add(1).logBase(goalFactor);
            gain = min(gain.floorNum(), maxGain);
            points += gain;
            magic = 0;
            if (points >= capacity) { magic = 0; points = capacity; }
            // g' = g * gF ^ gain
            goal *= goalFactor ^ gain;;
            std::stringstream ss;
            ss << "+" << gain << "UP";
            newMessage(ss.str(), BLUE);
        }
    }
}
void Catalyst::usePoints(Number amnt) {
    amnt = min(points, amnt);
    (points -= amnt).floorNum();
    goal = goalBase * (goalFactor ^ points);
    std::stringstream ss;
    ss << "-" << amnt << "UP";
    newMessage(ss.str(), RED);
}

// Multiplier Upgrade
std::vector<int> Catalyst::MultiU::TARGETS = {WIZARD, POWER_WIZARD};
Catalyst::MultiU::MultiU() : Upgrade(-1, "wizard", [&]() {
        return "Target: " + mImg + "\nx" + mMulti.toString(); }) {}
void Catalyst::MultiU::update(Timestep ts) {
    auto& catalyst = Game::get().wizards.catalyst;
    Number two = Number(2);
    mMulti = catalyst.getPoints() * two + catalyst.getPoints() ^ two / two;
    mMulti += max(1, catalyst.getMagic()).logTen();
    (mMulti += 1) ^= .3;
}
void Catalyst::MultiU::levelUp() {
    auto& wizards = Game::get().wizards;
    int oldIdx = mTIdx;
    mTIdx = ++mTIdx % TARGETS.size();
    while (mTIdx != oldIdx && !wizards.isVisible(TARGETS.at(mTIdx))) {
        mTIdx = ++mTIdx % TARGETS.size();
    }
    if (mTIdx != oldIdx) {
        mImg = wizards.getSprite(TARGETS.at(mTIdx)).getImage();
        updateMe = true;
    }
}
Number Catalyst::MultiU::getMulti(int id) {
    return Game::get().wizards.catalyst.visible() && id == TARGETS.at(mTIdx) ? mMulti : 1;
}
