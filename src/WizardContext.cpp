#include "WizardContext.h"
#include "Game.h"

// Wizard Context
void WizardContext::init() {
    AssetManager am = Game::assets();
    int w = Game::icon_w * 3 / 2;
    crystal.mRect = Rect::getMinRect(am.getAsset("crystal"), w, w);
    w = Game::icon_w / 2;
    catalyst.mRect = Rect::getMinRect(am.getAsset("catalyst"), w, w);
    w = Game::icon_w;
    wizard.mRect = Rect::getMinRect(am.getAsset("wizard"), w, w);

    double x = -crystal.mRect.w * 3 / 2;

    for (Sprite* s : mSprites) {
         s->mRect.setCenter(x, 0);
    }
    crystal.mRect.setCenterX(0);

    crystal.mVisible = true;
    wizard.mVisible = true;

    upgradeManager.init();
}
void WizardContext::update(Timestep ts) {
    // Custom update order? (or just custom update no function calls)
    for (Sprite* s : mSprites) {
        if (s->mVisible) { s->update(*this, ts); }
    }
    upgradeManager.update(ts);
}
void WizardContext::handleEvent(Event& e) {
    upgradeManager.handleEvent(e);
    for (auto it = mSprites.end(); it != mSprites.begin();) {
        --it;
        if (e.handled) { return; }
        if ((*it)->mVisible) { (*it)->handleEvent(*this, e); }
    }
}
void WizardContext::render() {
    for (Sprite* s : mSprites) {
        if (s->mVisible) { s->render(*this); }
    }
    upgradeManager.render();
}
Sprite& WizardContext::getSprite(int id) {
    switch (id) {
        case CATALYST: return catalyst; break;
        case WIZARD: return wizard; break;
        default: return crystal; break;
    }
    return crystal;
}
