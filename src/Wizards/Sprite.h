#ifndef SPRITE_h
#define SPRITE_h

#include <vector>

#include <SDL.h>

#include "../Rect.h"
#include "../Tools.h"
#include "../AssetManager.h"
#include "Upgrade.h"

class Sprite {
public:
    bool mVisible = false, mDragging = false;
    Rect mRect;

    Sprite() = default;
    ~Sprite() = default;

    virtual void init();
    virtual void update(Timestep ts);
    virtual void handleEvent(Event& e) {}
    virtual void render();

    virtual std::string getImage() { return ""; }
    std::vector<Upgrade*>& getUpgrades() { return mUpgrades; }

    static bool drag(Sprite& s, Event& e);
    static bool noDrag(Sprite& s, Event& e);
    void addMessage(std::string text, SDL_Color color = BLACK);
protected:
    std::vector<Upgrade*> mUpgrades;
private:
    struct Message {
        SDL_Texture* tex;
        Rect rect;
        int timer = 2500;

        void destroy() {
            if(tex != nullptr) { SDL_DestroyTexture(tex); }
        }
    };
    double mMessageY = 0.;
    TextData mText;
    std::vector<Message> mMessages;
};

#endif /* SPRITE_h */
