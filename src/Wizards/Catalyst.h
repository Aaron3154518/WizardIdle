#ifndef CATALYST_h
#define CATALYST_h

#include <vector>

#include "../Rect.h"
#include "../Number.h"
#include "../Tools.h"
#include "../AssetManager.h"
#include "Sprite.h"
#include "Upgrade.h"

class Catalyst : public Sprite {
public:
    Catalyst();

    void init();
//    void update(Timestep ts);
    void handleEvent(Event& e);
    void render();

    std::string getImage() { return "catalyst"; }

    void addMagic(Number add);
    void usePoints(Number amnt);
    Number getMagic() { return magic; }
    Number getPoints() { return points; }
private:
    Number magic, points;
    Number capacity = Number(10), maxGain = Number(1);
    Number goalBase = Number(50), goal = goalBase.copy(), goalFactor = Number(2.5);
    TextData pointsText, magicText;
    class MultiU : public Upgrade {
    public:
        MultiU();

        void init() { setDescription("Applies a multiplier to one wizard base on UP\nClick to change the target wizard"); }
        void update(Timestep ts);
        void levelUp();
        Number getMulti(int id);
    private:
        static std::vector<int> TARGETS;
        int mTIdx = 0;
        Number mMulti = Number(1);
    };
public:
    MultiU multi_u;
};

#endif /* CATALYST_h */
