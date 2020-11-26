#include "PowerWizard.h"
#include "WizardData.h"
#include "../Game.h"

// Power Wizard
PowerWizard::PowerWizard() : mFireballs(FireballHandler({WIZARD})) {
    mFireballs.setImage("fireball2");
}
void PowerWizard::init() {
    int w = Game::get().icon_w;
    mRect = Rect::getMinRect(Game::get().assets.getAsset(getImage()), w, w);
    mRect.setCenter(-Game::get().icon_w * 2, 0.);

    mUpgrades = std::vector<Upgrade*> {};
    Sprite::init();
}
void PowerWizard::update(Timestep ts) {
    std::vector<Fireball> vec = mFireballs.update(ts);
    for (Fireball& f : vec) {
        std::stringstream ss;
        switch (f.mTarget) {
            case WIZARD:
                ss << "+" << power << "x";
                Game::get().wizards.wizard.addMessage(ss.str(), CYAN);
                Game::get().wizards.wizard.basePower = power;
                break;
        }
    }
    vec.clear();
    if (mFireballs.ready()) {
        mFireballs.newFireball(mRect.cX(), mRect.cY(), power);
    }

    Sprite::update(ts);
}
void PowerWizard::handleEvent(Event& e) {
    if (Sprite::drag(*this, e)) { Game::get().wizards.upgradeManager.select(POWER_WIZARD); }
}
void PowerWizard::render() {
    Rect r = Game::get().getAbsRect(mRect);
    Game::get().assets.drawTexture(getImage(), r, NULL);
    mFireballs.render();

    Sprite::render();
}
