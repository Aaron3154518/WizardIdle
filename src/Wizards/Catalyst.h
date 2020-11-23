#ifndef CATALYST_h
#define CATALYST_h

#include "../Rect.h"
#include "../Number.h"
#include "../Tools.h"
#include "Sprite.h"
#include "Upgrade.h"

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

#endif /* CATALYST_h */
