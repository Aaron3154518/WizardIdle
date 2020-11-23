#include "WizardContext.h"
#include "Game.h"

// Wizard Context
void WizardContext::init() {
    AssetManager am = Game::assets();
    for (Sprite* s : mSprites) { s->init(); }
    upgradeManager.init();
}
void WizardContext::update(Timestep ts) {
    // Custom update order? (or just custom update no function calls)
    for (Sprite* s : mSprites) {
        if (s->mVisible) { s->update(*this, ts); }
    }
    upgradeManager.update(*this, ts);
}
void WizardContext::handleEvent(Event& e) {
    upgradeManager.handleEvent(*this, e);
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
