#ifndef WIZARD_h
#define WIZARD_h

#include <vector>
#include <array>

#include "../Rect.h"
#include "../Number.h"
#include "../Tools.h"
#include "Sprite.h"
#include "Upgrade.h"
#include "Fireball.h"

class Wizard : public Sprite {
public:
    using Sprite::Sprite;
 
    Number basePower = 1., power = 1.;

    Wizard(int id);

    void init();
    void update(Timestep ts);
    void handleEvent(Event& e);
    void render();

    std::string getImage() { return "wizard"; }
private:
    const static std::vector<int> TARGETS;
    FireballHandler mFireballs;
};

#endif /* WIZARD_h */
