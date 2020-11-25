#ifndef UPGRADES_h
#define UPGRADES_h

#include <string>
#include <sstream>
#include <vector>
#include <functional>

#include <SDL.h>

#include "../Rect.h"
#include "../Game.h"
#include "../AssetManager.h"

typedef std::function<std::string()> InfoFunc;

class Upgrade {
public:
    bool updateMe = false;

    Upgrade(int maxLevel, std::string img, InfoFunc func = []() { return "No Text Provided"; }) :
        mMaxLevel(maxLevel), mImg(img), getInfo(func) {}
    Upgrade(InfoFunc func) : getInfo(func) {}
    ~Upgrade();

    virtual void init () { setDescription("No Description Provided"); }
    virtual void update(Timestep ts) {}
    virtual void levelUp() { ++mLevel; }
    void render(Rect& r);
    void renderDescription(SDL_Point pos);
    void setDescription(std::string desc);

    bool maxLevel() { return mLevel == mMaxLevel; }
    bool visible() { return mVisible; }
    void toggleVisibility() { mVisible = !mVisible; updateMe = true; }

    virtual bool canBuy() { return mMaxLevel == -1 || mLevel < mMaxLevel; }
protected:
    // Max Level >0: caps at l, =0: not upgradeable, <0: no cap
    int mLevel = 0, mMaxLevel = 0;
    bool mVisible = false;

    std::string mImg = "";
private:
    TextData infoText;
    InfoFunc getInfo;
    Rect mDescRect;
    SDL_Texture* mDesc;
};

class UpgradeVector {
public:
    UpgradeVector() = default;
    UpgradeVector(std::vector<Upgrade*> upgrades);
    ~UpgradeVector() = default;
    void operator=(std::vector<Upgrade*> upgrades);

    void init();
    void update(Timestep ts);

    Upgrade* at(int idx);
    void push_back(Upgrade* u);

    void click(int idx);
    bool isUpdate();
    std::vector<int> getVisible();
private:
    std::vector<Upgrade*> mUpgrades;
};

class UpgradeManager{
public:
    UpgradeManager() = default;
    ~UpgradeManager();

    void init();
    void update(Timestep ts);
    void handleEvent(Event& e);
    void render();

    void select(int id);
    void scroll(double ds);
private:
    bool mDragging = false;
    double mScroll = 0., mScrollV = 0.;
    int mSelected = -1;
    std::map<int, Rect> mFrontRects, mBackRects;
    Rect mRect;
    UpgradeVector mUVec;
    SDL_Texture* mTex;

    void redraw();
};

#endif /* UPGRADES_h */
