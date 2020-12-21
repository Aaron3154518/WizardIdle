#ifndef CRYSTAL_h
#define CRYSTAL_h

#include "../Definitions.h"
#include "../Rect.h"
#include "../Number.h"
#include "../Tools.h"
#include "../AssetManager.h"
#include "Sprite.h"
#include "Upgrade.h"

class Crystal : public Sprite {
public:
    Crystal();

    Number magic;

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
    class UnlockU : public Upgrade {
    public:
        UnlockU(int id);

        void init();
        void levelUp();
        bool canBuy();
    private:
        int mId;
    };
public:
    MultU mult_u;
    UnlockU wizard_u = UnlockU(WIZARD);
    UnlockU catalyst_u = UnlockU(CATALYST);
    UnlockU powerWizard_u = UnlockU(POWER_WIZARD);
};

#endif /* CRYSTAL_h */
