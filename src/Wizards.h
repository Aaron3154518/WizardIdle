#ifndef WIZARDS_h
#define WIZARDS_h

#include <vector>
#include <sstream>

#include <SDL.h>

#include "Definitions.h"
#include "Rect.h"
#include "Number.h"
#include "Tools.h"
#include "Fireball.h"
#include "Upgrade.h"
#include "Game.h"
#include "AssetManager.h"

// Forward Declaration
class WizardContext;

class Sprite {
public:
    bool mVisible = false, mDragging = false;
    Rect mRect;

    Sprite() = default;
    ~Sprite() = default;

    virtual void init() {}
    virtual void update(Timestep ts) {}
    virtual void handleEvent(Event& e) {}
    virtual void render() {}

    virtual std::string getImage() { return ""; }
    std::vector<Upgrade*>& getUpgrades() { return mUpgrades; }
protected:
    std::vector<Upgrade*> mUpgrades;
};

class Crystal : public Sprite {
public:
    Number magic, power;

    Crystal() = default;
    ~Crystal() = default;

    void init();
    void update(Timestep ts);
    void handleEvent(Event& e);
    void render();

    std::string getImage() { return "crystal"; }
private:
    Number cost;
    class WizardU : public Upgrade {
    public:
        WizardU() : Upgrade([&]() {return mLevel == 1 ? "Bought" : "Free"; }) {}

        void init() { Upgrade::init(1, "wizard", "Unlock Wizard\nWizard shoots fireballs at the crystal"); }
        void levelUp();
    };
    class CatalystU : public Upgrade {
    public:
        CatalystU();

        void init() { Upgrade::init(1, "catalyst", "Unlock Catalyst\nThe wizard can shoot fireballs "
                "into the catalyst to fill it up. Use catalyst power to power up your wizards!"); }
        void levelUp();
        bool canBuy();
    };
public:
    WizardU wizard_u;
    CatalystU catalyst_u;
};
class Catalyst : public Sprite {
public:
    Number capacity, power;

    Catalyst() = default;
    ~Catalyst() = default;

    void init();
    void update(Timestep ts);
    void handleEvent(Event& e);
    void render();

    std::string getImage() { return "catalyst"; }

    void addMagic(Number& add) {
        magic = Number::getMin(capacity, magic + add);
    }
    const Number& getMagic() { return magic; }
private:
    Number magic;
};
class Wizard : public Sprite {
public:
    Number basePower = 1., power = 1.;

    Wizard();
    ~Wizard() = default;

    void init();
    void update(Timestep ts);
    void handleEvent(Event& e);
    void render();

    std::string getImage() { return "wizard"; }
private:
    FireballHandler mFireballs;
};


#endif /* WIZARDS_h */
