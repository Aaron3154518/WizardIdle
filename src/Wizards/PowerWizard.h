#ifndef POWER_WIZARD_h
#define POWER_WIZARD_h

#include <unordered_map>

#include "../Rect.h"
#include "../Number.h"
#include "../Tools.h"
#include "../AssetManager.h"
#include "Sprite.h"
#include "Fireball.h"

class PowerWizard : public Sprite {
public:
    PowerWizard();

    void init();
    void update(Timestep ts);
    void handleEvent(Event& e);
    void render();

    Number getMulti(int id) { return mMultis.at(id); }

    std::string getImage() { return "power_wizard"; }
private:
    Number power = Number(5);
    std::unordered_map<int, Number> mMultis;
    std::unordered_map<int, MessagePtr> mMultiMessages;
    const static std::vector<int> TARGETS;
    FireballHandler mFireballs;
};

#endif /* POWER_WIZARD_h */
