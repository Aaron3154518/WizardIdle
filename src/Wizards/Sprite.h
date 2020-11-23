#ifndef SPRITE_h
#define SPRITE_h

#include <vector>

#include "../Rect.h"
#include "../Tools.h"
#include "Upgrade.h"

class Sprite {
public:
    bool mVisible = false, mDragging = false;
    Rect mRect;

    Sprite() = default;
    ~Sprite() = default;

    virtual void init() {}
    virtual void update(Timestep ts) {}
    virtual void handleEvent(Event& e) {}
    virtual void render() {}

    virtual std::string getImage() { return ""; }
    std::vector<Upgrade*>& getUpgrades() { return mUpgrades; }

    static bool drag(Sprite& s, Event& e);
    static bool noDrag(Sprite& s, Event& e);
protected:
    std::vector<Upgrade*> mUpgrades;
};

#endif /* SPRITE_h */
