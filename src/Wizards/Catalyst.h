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
    Number points;

    Catalyst() = default;
    ~Catalyst() = default;

    void init();
    void update(Timestep ts);
    void handleEvent(Event& e);
    void render();

    std::string getImage() { return "catalyst"; }

    void addMagic(Number add);
    const Number& getMagic() { return magic; }
private:
    Number capacity = 10, benchmark = 100, magic;
    TextData pointsText, magicText;
};

#endif /* CATALYST_h */
