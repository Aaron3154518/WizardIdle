#ifndef WIZARDCONTEXT_h
#define WIZARDCONTEXT_h

#include <iostream>
#include <vector>


#include "../Tools/Tools.h"
#include "Sprite.h"
#include "Crystal.h"
#include "Catalyst.h"
#include "Wizard.h"
#include "PowerWizard.h"
#include "Upgrade.h"

class WizardData {
public:
    UpgradeManager upgradeManager;

    Crystal crystal;
    Catalyst catalyst;
    Wizard wizard;
    PowerWizard powerWizard;

    WizardData() = default;
    ~WizardData() = default;

    void init();
    void clean();
    void update(Timestep ts);
    void handleEvent(Event& e);
    void render();
    
    // Getters
    Sprite& getSprite(int id);
    bool isVisible(int id) { return getSprite(id).visible(); }
private:
    std::vector<Sprite*> mSprites = { &crystal, &catalyst, &wizard, &powerWizard };
};

#endif /* WIZARDCONTEXT_h */
