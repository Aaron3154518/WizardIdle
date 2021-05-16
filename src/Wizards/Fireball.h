#ifndef FIREBALL_h
#define FIREBALL_h

#include <string>
#include <vector>
//#include <initializer_list>

#include <SDL.h>

#include "../Tools/Rect.h"
#include "../Tools/Number.h"
#include "../Tools/Tools.h"
#include "../AssetManager.h"
#include "Upgrade.h"

// Forward Declaration
class WizardData;

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

class FireballHandler : public Upgrade {
public:
    Number power;

    FireballHandler(int spriteId, std::vector<int> targets);
    ~FireballHandler() = default;

    void init() { setDescription("Select where to shoot fireballs"); updateImage(); }
    void levelUp();
    void update(Timestep ts);
    void render();

    std::vector<Fireball> getCollided();

    void setDelay(int delay) { mTimer = mDelay = delay; }

    void setFireballImage(std::string img) { mFImg = img; }
    int getTarget() { return mTargets.size() == 0 ? -1 : mTargets.at(mTIdx); }
    void setTarget(int target);
private:
    std::string mFImg = "fireball";
    int mSId = 0, mTIdx = 0;
    std::vector<int> mTargets;
    int mTimer = 0, mDelay = 50 * pow(1.25, 20) / 3;
    std::vector<Fireball> mFireballs, mCollided;

    void updateImage();
};
#endif /* FIREBALL_h */
