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

class Wizards {
public:
    static void init();
    static void update(Timestep ts);
    static void handleEvent(Event& e);
    static void render();
    
    // Getters
    static Sprite& getSprite(int id);
    static Crystal& crystal();
    static Catalyst& catalyst();
    static Wizard& wizard();
    static PowerWizard& powerWizard();
    static UpgradeManager& upgradeManager();
};

#endif /* WIZARDCONTEXT_h */
