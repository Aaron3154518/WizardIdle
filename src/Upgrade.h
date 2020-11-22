#ifndef UPGRADES_h
#define UPGRADES_h

#include <string>
#include <sstream>
#include <vector>

#include <SDL.h>

#include "Rect.h"
#include "Game.h"
#include "AssetManager.h"

class Upgrade {
public:
    Upgrade() = default;
    ~Upgrade();

    void init (int maxLevel, std::string img, std::string desc);
    virtual void update(Timestep ts) {}
    virtual void levelUp() { ++mLevel; }
    void render(Rect& r);
    void renderDescription(SDL_Point pos);

    virtual std::string getCostString() { return ""; }
    virtual std::string getEffectString() { return ""; }
    virtual bool canBuy() { return false; }
private:
    // Max Level >0: caps at l, =0: not upgradeable, <0: no cap
    int mLevel = 0, mMaxLevel = 0;

    std::string mImg = "";
    Rect mDescRect;
    SDL_Texture* mDesc;
};

class UpgradeManager{
public:
    UpgradeManager() = default;
    ~UpgradeManager();

    void init();
    void update(Timestep ts);
    void handleEvent(Event& e);
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
