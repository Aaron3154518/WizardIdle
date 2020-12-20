#ifndef CATALYST_h
#define CATALYST_h

#include "../Rect.h"
#include "../Number.h"
#include "../Tools.h"
#include "../AssetManager.h"
#include "Sprite.h"
#include "Upgrade.h"

class Catalyst : public Sprite {
public:
    using Sprite::Sprite;

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
    Number capacity = Number(10), magic, points;
    Number goal = Number(1,2), goalFactor = Number(2.5);
    TextData pointsText, magicText;
};

#endif /* CATALYST_h */
