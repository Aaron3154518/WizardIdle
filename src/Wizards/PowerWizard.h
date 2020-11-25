#ifndef POWER_WIZARD_h
#define POWER_WIZARD_h

#include "../Rect.h"
#include "../Number.h"
#include "../Tools.h"
#include "../AssetManager.h"
#include "Sprite.h"
#include "Fireball.h"

class PowerWizard : public Sprite {
public:
    PowerWizard();
    ~PowerWizard() = default;

    void init();
    void update(Timestep ts);
    void handleEvent(Event& e);
    void render();

    std::string getImage() { return "power_wizard"; }
private:
    Number power = Number(1, 2);
    FireballHandler mFireballs;

public:
};

#endif /* POWER_WIZARD_h */
