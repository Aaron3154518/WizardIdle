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

        MultU() :
            Upgrade([&]() {
                    std::stringstream ss;
                    ss << effect << "x";
                    return ss.str();
                    }) {}
        void init() { Upgrade::init(0, "crystal", "Crystal multiplies gains of all wizards based on crystal magic"); }
        void update(Timestep ts);
    };
    class WizardU : public Upgrade {
    public:
        WizardU() : Upgrade([&]() {return mLevel == 1 ? "Bought" : "Free"; }) {}

        void init() { Upgrade::init(1, "wizard", "Unlock Wizard\nWizard shoots fireballs at the crystal, increasing its magic (M)"); }
        void levelUp();
    };
    class CatalystU : public Upgrade {
    public:
        CatalystU();

        void init() { Upgrade::init(1, "catalyst", "Unlock Catalyst\nThe wizard can shoot fireballs "
                "into the catalyst to fill it up and gain Upgrade Points (UP). Use UP to power up your wizards!"); }
        void levelUp();
        bool canBuy();
    };
public:
    MultU mult_u;
    WizardU wizard_u;
    CatalystU catalyst_u;
};

#endif /* CRYSTAL_h */
