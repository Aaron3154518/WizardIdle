#ifndef WIZARDCONTEXT_h
#define WIZARDCONTEXT_h

#include <iostream>
#include <vector>

#include "../Tools.h"
#include "Sprite.h"
#include "Crystal.h"
#include "Catalyst.h"
#include "Wizard.h"
#include "PowerWizard.h"
#include "Upgrade.h"

constexpr int CRYSTAL = 0;
constexpr int CATALYST = 1;
constexpr int WIZARD = 2;
constexpr int POWER_WIZARD = 3;

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
private:
    std::vector<Sprite*> mSprites = { &crystal, &catalyst, &wizard, &powerWizard };
};

#endif /* WIZARDCONTEXT_h */
