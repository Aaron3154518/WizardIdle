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
#include "Game.h"
#include "Upgrade.h"
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
    virtual void update(WizardContext& wc, Timestep ts) {}
    virtual void handleEvent(WizardContext& wc, Event& e) {}
    virtual void render(WizardContext& wc) {}
protected:
    std::vector<Upgrade*> mUpgrades;
};

class Crystal : public Sprite {
public:
    Number magic, power;

    Crystal() { mUpgrades.push_back(&wizard_u); }
    ~Crystal() = default;

    void init();
    void update(WizardContext& wc, Timestep ts);
    void handleEvent(WizardContext& wc, Event& e);
    void render(WizardContext& wc);
private:
    class WizardU : public Upgrade {
    public:
        WizardU() : Upgrade([&]() {return bought ? "Bought" : "Free"; }) {};
        ~WizardU() = default;

        void init() { Upgrade::init(1, "wizard", "Unlocks Wizard"); }
        void levelUp(WizardContext& wc);
    private:
        bool bought = false;
        Number cost;
    };
public:
    WizardU wizard_u;
};
class Catalyst : public Sprite {
public:
    Number capacity, power;

    Catalyst() = default;
    ~Catalyst() = default;

    void init();
    void update(WizardContext& wc, Timestep ts);
    void handleEvent(WizardContext& wc, Event& e);
    void render(WizardContext& wc);

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
    void update(WizardContext& wc, Timestep ts);
    void handleEvent(WizardContext& wc, Event& e);
    void render(WizardContext& wc);
private:
    FireballHandler mFireballs;
};


#endif /* WIZARDS_h */
