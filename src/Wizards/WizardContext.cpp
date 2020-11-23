#include "WizardContext.h"
#include "../Game.h"

struct WizardData {
    friend class Wizards;
private:
    UpgradeManager upgradeManager;

    Crystal crystal;
    Catalyst catalyst;
    Wizard wizard;

    bool initialized = false;
    std::vector<Sprite*> mSprites = { &crystal, &catalyst, &wizard };
};

WizardData wc;

void Wizards::init() {
    if ( wc.initialized ) { return; }
    for (Sprite* s : wc.mSprites) { s->init(); }
    wc.upgradeManager.init();
}
void Wizards::update(Timestep ts) {
    // Custom update order? (or just custom update no function calls)
    for (Sprite* s : wc.mSprites) {
        if (s->mVisible) { s->update(ts); }
    }
    wc.upgradeManager.update(ts);
}
void Wizards::handleEvent(Event& e) {
    wc.upgradeManager.handleEvent(e);
    for (auto it = wc.mSprites.end(); it != wc.mSprites.begin();) {
        --it;
        if (e.handled) { return; }
        if ((*it)->mVisible) { (*it)->handleEvent(e); }
    }
}
void Wizards::render() {
    for (Sprite* s : wc.mSprites) {
        if (s->mVisible) { s->render(); }
    }
    wc.upgradeManager.render();
}

// Getters
Sprite& Wizards::getSprite(int id) {
    switch (id) {
        case CATALYST: return wc.catalyst; break;
        case WIZARD: return wc.wizard; break;
        default: return wc.crystal; break;
    }
}
Crystal& Wizards::crystal() { return wc.crystal; }
Catalyst& Wizards::catalyst() { return wc.catalyst; }
Wizard& Wizards::wizard() { return wc.wizard; }
UpgradeManager& Wizards::upgradeManager() { return wc.upgradeManager; }
