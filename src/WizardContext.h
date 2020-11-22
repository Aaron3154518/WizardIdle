#ifndef WIZARDCONTEXT_h
#define WIZARDCONTEXT_h

#include <iostream>
#include <vector>

#include "Definitions.h"
#include "Tools.h"
#include "AssetManager.h"
#include "Wizards.h"
#include "Upgrade.h"

constexpr int CRYSTAL = 0;
constexpr int CATALYST = 1;
constexpr int WIZARD = 2;

class WizardContext {
public:
    UpgradeManager upgradeManager;

    Crystal crystal;
    Catalyst catalyst;
    Wizard wizard;

    WizardContext() = default;
    ~WizardContext() = default;

    void init();
    void update(Timestep ts);
    void handleEvent(Event& e);
    void render();
    Sprite& getSprite(int id);
private:
    std::vector<Sprite*> mSprites = { &crystal, &catalyst, &wizard };
};

#endif /* WIZARDCONTEXT_h */
