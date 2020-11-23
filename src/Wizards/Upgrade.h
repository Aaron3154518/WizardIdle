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
    Upgrade() = default;
    Upgrade(InfoFunc func) : getInfo(func) {}
    ~Upgrade();

    void init (int maxLevel, std::string img, std::string desc);
    virtual void update(Timestep ts) {}
    virtual void levelUp() { ++mLevel; }
    void render(Rect& r);
    void renderDescription(SDL_Point pos);
    bool maxLevel() { return mLevel == mMaxLevel; }

    virtual bool canBuy() { return mMaxLevel == -1 || mLevel < mMaxLevel; }
protected:
    // Max Level >0: caps at l, =0: not upgradeable, <0: no cap
    int mLevel = 0, mMaxLevel = 0;

    std::string mImg = "";
private:
    InfoFunc getInfo = [&]() { return "No Text Provided"; };
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

    void setSprite(int id);
    void scroll(double ds);

private:
    bool mDragging = false;
    double mScroll = 0., mScrollMax = 0., mScrollV = 0.;
    int mSelected = -1, mSize = 0;
    std::map<int, Rect> mURects;
    Rect mRect;
    SDL_Texture* mTex;

    void redraw();
    std::vector<Upgrade*> getUpgrades();
};

#endif /* UPGRADES_h */
