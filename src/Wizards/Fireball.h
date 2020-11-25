#ifndef FIREBALL_h
#define FIREBALL_h

#include <string>
#include <vector>
//#include <initializer_list>

#include <SDL.h>

#include "../Rect.h"
#include "../Number.h"
#include "../Tools.h"
#include "../AssetManager.h"

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
    FireballHandler() = default;
    FireballHandler(std::vector<int> targets) : mTargets(targets) {}
/*    FireballHandler(std::initializer_list<int> targets) {
        for (int i : targets) {mTargets.push_back(i);}
    }*/
    ~FireballHandler() = default;

    std::vector<Fireball> update(Timestep ts);
    void render();

    void setDelay(int delay) { mTimer = mDelay = delay; }
    bool ready() { return mTimer <= 0; }
    void newFireball(double x, double y, Number data);

    void setImage(std::string img) { mImg = img; }
    int nextTarget();
    int getTarget() { return mTargets.size() == 0 ? -1 : mTargets.at(mTIdx); }
    void setTarget(int target);
private:
    std::string mImg = "fireball";
    int mTIdx = 0;
    std::vector<int> mTargets;
    int mTimer = 0, mDelay = 50 * pow(1.25, 20) / 3;
    std::vector<Fireball> mFireballs;
};


#endif /* FIREBALL_h */
