#include "WizardData.h"
#include "../Game.h"

void WizardData::init() {
    for (Sprite* s : mSprites) { s->init(); }
    upgradeManager.init();
}
void WizardData::clean() {
    std::cout << "Implement Clean" << std::endl; 
}
void WizardData::update(Timestep ts) {
    // Custom update order? (or just custom update no function calls)
    for (Sprite* s : mSprites) {
        if (s->visible()) { s->update(ts); }
    }
    upgradeManager.update(ts);
}
void WizardData::handleEvent(Event& e) {
    upgradeManager.handleEvent(e);
    for (auto it = mSprites.end(); it != mSprites.begin();) {
        --it;
        if (e.handled) { return; }
        if ((*it)->visible()) { (*it)->handleEvent(e); }
    }
}
void WizardData::render() {
    for (Sprite* s : mSprites) {
        if (s->visible()) { s->render(); }
    }
    upgradeManager.render();
}

// Getters
Sprite& WizardData::getSprite(int id) {
    switch (id) {
        case CATALYST: return catalyst; break;
        case WIZARD: return wizard; break;
        case POWER_WIZARD: return powerWizard; break;
        default: return crystal; break;
    }
}
