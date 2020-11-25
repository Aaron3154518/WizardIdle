#include "PowerWizard.h"
#include "WizardContext.h"
#include "../Game.h"

// Power Wizard
PowerWizard::PowerWizard() : mFireballs(FireballHandler({WIZARD})) {
    mFireballs.setImage("fireball2");
}
void PowerWizard::init() {
    Sprite::init();
}
void PowerWizard::update(Timestep ts) {
    Sprite::update(ts);
}
void PowerWizard::handleEvent(Event& e) {

}
void PowerWizard::render() {
    Sprite::render();
}
