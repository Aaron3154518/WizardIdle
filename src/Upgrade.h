#ifndef UPGRADES_h
#define UPGRADES_h

#include <string>
#include <sstream>
#include <vector>
#include <functional>

#include <SDL.h>

#include "Rect.h"
#include "Game.h"
#include "AssetManager.h"

// Forward Declaration
class WizardContext;

typedef std::function<std::string()> StringFunc;

class Upgrade {
public:
    Upgrade() = default;
    Upgrade(StringFunc func) : getDynamicDescription(func) {}
    ~Upgrade();

    void init (int maxLevel, std::string img, std::string desc);
    virtual void update(WizardContext& wc, Timestep ts) {}
    virtual void levelUp(WizardContext& wc) { ++mLevel; }
    void render(Rect& r);
    void renderDescription(SDL_Point pos);

    virtual bool maxLevel() { return mLevel == mMaxLevel; }
    virtual bool canBuy(WizardContext& wc) { return mMaxLevel == -1 || mLevel < mMaxLevel; }
protected:
    // Max Level >0: caps at l, =0: not upgradeable, <0: no cap
    int mLevel = 0, mMaxLevel = 0;

    std::string mImg = "";
    StringFunc getDynamicDescription = [&]() { return "No Text Provided"; };
private:
    Rect mDescRect;
    SDL_Texture* mDesc;
};

class UpgradeManager{
public:
    UpgradeManager() = default;
    ~UpgradeManager();

    void init();
    void update(WizardContext& wc, Timestep ts);
    void handleEvent(WizardContext& wc, Event& e);
    void render();

    void setUpgrades(std::vector<Upgrade*>* newList);
    void scroll(double ds);

private:
    bool mDragging = false;
    double mScroll = 0., mScrollMax = 0., mScrollV = 0.;
    std::vector<Upgrade*>* mUpgrades;
    std::map<int, Rect> mURects;
    Rect mRect;
    SDL_Texture* mTex;

    void redraw();
};

#endif /* UPGRADES_h */
