#ifndef WIZARD_h
#define WIZARD_h

#include <array>

#include "../Rect.h"
#include "../Number.h"
#include "../Tools.h"
#include "Sprite.h"
#include "Upgrade.h"
#include "Fireball.h"

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
    class TargetU : public Upgrade {
    public:
        TargetU();

        void init() { Upgrade::init(-1, "crystal", "Select where to shoot fireballs"); }
        void levelUp();
    private:
        int mTarget;
    };
public:
    TargetU target_u;
};

#endif /* WIZARD_h */
