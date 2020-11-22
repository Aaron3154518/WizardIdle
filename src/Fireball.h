#ifndef FIREBALL_h
#define FIREBALL_h

#include <string>
#include <vector>

#include <SDL.h>

#include "Definitions.h"
#include "Rect.h"
#include "Number.h"
#include "Tools.h"
#include "AssetManager.h"

// Forward Declaration
class WizardContext;

class Fireball {
public:
    double maxV = 1.5;
    Rect mRect;
    Number mData;
    int mTarget = -1;

    Fireball() { setPos(0, 0); };
    Fireball(double _x, double _y) { setPos(_x, _y); }
    ~Fireball() = default;

    bool update(Rect& target, Timestep ts);
    void setPos(double _x, double _y);
    void setImage(std::string img);
    void setSize(int size);
    std::string getImage() { return mImg; }
private:
    double x = 0., y = 0.;
    int mSize = 50;
    std::string mImg = "fireball";
};

class FireballHandler {
public:
    int mTarget = -1;
    std::string mImg = std::string("fireball");

    FireballHandler() = default;
    FireballHandler(int target) : mTarget(target) {}
    ~FireballHandler() = default;

    std::vector<Fireball> update(WizardContext& wc, Timestep ts);
    void render();

    void setDelay(int delay) { mTimer = mDelay = delay; }
    bool ready() { return mTarget != -1 && mTimer <= 0; }
    void newFireball(double x, double y, Number data);
private:
    int mTimer = 0, mDelay = 50 * pow(1.25, 20) / 3;
    std::vector<Fireball> mFireballs;
};


#endif /* FIREBALL_h */
