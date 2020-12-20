#include "PowerWizard.h"
#include "WizardData.h"
#include "../Game.h"

// Power Wizard
const std::vector<int> PowerWizard::TARGETS = {WIZARD};

PowerWizard::PowerWizard(int id) : Sprite(id), mFireballs{id, TARGETS} {
    mFireballs.setFireballImage("fireball2");
    mFireballs.setDelay(5000);
    for (int id : TARGETS) { mMultis[id] = 1; }
}
void PowerWizard::init() {
    int w = Game::get().icon_w;
    mRect = Rect::getMinRect(Game::get().assets.getAsset(getImage()), w, w);
    mRect.setCenter(-Game::get().icon_w * 2, 0.);

    mFireballs.toggleVisibility();
    mUpgrades = std::vector<Upgrade*> { &mFireballs };
    Sprite::init();
}
void PowerWizard::update(Timestep ts) {
    for (auto it = mMultis.begin(); it != mMultis.end(); ++it) {
        if (it->second > 1) {
            auto& m = mMultiMessages[it->first];
            it->second *= pow(.8, ts.GetSeconds());
            if (it->second <= 1) {
                it->second = 1;
                m->mActive = false;
            } else {
                std::stringstream ss;
                ss << "x" << it->second;
                m->setText(ss.str());
            }
       }
    }

    mFireballs.power = power;
    for (Fireball& f : mFireballs.getCollided()) {
        std::stringstream ss;
        switch (f.mTarget) {
            case WIZARD:
                mMultis[WIZARD] = power;
                ss << "x" << power;
                auto& m = mMultiMessages[WIZARD];
                if (!m || !m->mActive) {
                    Wizard& w = Game::get().wizards.wizard;
                    m = w.newMessage(ss.str(), BLUE, -1);
                }
                break;
        }
    }

    Sprite::update(ts);
}
void PowerWizard::handleEvent(Event& e) {
    drag(*this, e);
}
void PowerWizard::render() {
    Rect r = Game::get().getAbsRect(mRect);
    Game::get().assets.drawTexture(getImage(), r, NULL);
    mFireballs.render();

    Sprite::render();
}
