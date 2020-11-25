#ifndef CRYSTAL_h
#define CRYSTAL_h

#include "../Rect.h"
#include "../Number.h"
#include "../Tools.h"
#include "../AssetManager.h"
#include "Sprite.h"
#include "Upgrade.h"

class Crystal : public Sprite {
public:
    Number magic;

    Crystal() = default;
    ~Crystal() = default;

    void init();
//    void update(Timestep ts);
    void handleEvent(Event& e);
    void render();

    std::string getImage() { return "crystal"; }
private:
    TextData magicText;
    Number cost;
    class MultU : public Upgrade {
    public:
        Number effect;

        MultU();

        void init() { setDescription("Crystal multiplies gains of all wizards based on crystal magic"); }
        void update(Timestep ts);
    };
    class WizardU : public Upgrade {
    public:
        WizardU();

        void init() { setDescription("Unlock Wizard\nWizard shoots fireballs at the crystal, increasing its magic (M)"); }
        void levelUp();
    };
    class CatalystU : public Upgrade {
    public:
        CatalystU();

        void init() { setDescription("Unlock Catalyst\nThe wizard can shoot fireballs "
                "into the catalyst to fill it up and gain Upgrade Points (UP). Use UP to power up your wizards!"); }
        void levelUp();
        bool canBuy();
    };
    class PowerWizardU : public Upgrade {
    public:
        PowerWizardU();

        void init() { setDescription("Unlock Power Wizard\nThis wizard shoots fireballs "
                "at other wizards which gives them a boost that decays over time"); }
        void levelUp();
        bool canBuy();
    };
public:
    MultU mult_u;
    WizardU wizard_u;
    CatalystU catalyst_u;
    PowerWizardU powerWizard_u;
};

#endif /* CRYSTAL_h */
