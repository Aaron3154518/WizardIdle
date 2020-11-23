#include "Catalyst.h"
#include "WizardContext.h"

// Catalyst
void Catalyst::init() {
    int w = Game::icon_w * 3 / 4;
    mRect = Rect::getMinRect(Game::assets().getAsset(getImage()), w, w);
    mRect.setCenter(-Game::icon_w * 2, 0.);
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
}
