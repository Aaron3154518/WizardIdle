#ifndef SPRITE_h
#define SPRITE_h

#include <vector>
#include <memory>

#include <SDL.h>

#include "../Rect.h"
#include "../Tools.h"
#include "../AssetManager.h"
#include "Upgrade.h"

class Message {
public:
    bool mActive = true;
    int mTimer = 2500;
    double mY = 0.;
    Rect mRect;
  
    Message(std::string text = "Hello World", SDL_Color color = BLACK, int ms = 2500);
    Message(const Message& m);
    Message& operator=(const Message& m);
    ~Message() { destroy(); }

    SDL_Texture* getTexture() { return mTex; }
    void setText(std::string text) { mText.text = text; redraw(); }
    void setColor(SDL_Color color) { mText.color = color; redraw(); }
    bool permanent() { return mTimer == -1; }
    double bottom() { return mY + mRect.h; }
private:
    TextData mText;
    SDL_Texture* mTex = nullptr;

    void redraw();
    void destroy();
};

typedef std::shared_ptr<Message> MessagePtr;

class Sprite {
public:
    const int mId = 0;
    bool mVisible = false;
    Rect mRect;

    Sprite(int id) : mId(id) {}
    ~Sprite() = default;

    virtual void init();
    virtual void update(Timestep ts);
    virtual void handleEvent(Event& e) {}
    virtual void render();

    virtual std::string getImage() { return ""; }
    UpgradeVector& getUpgrades() { return mUpgrades; }

    static void drag(Sprite& s, Event& e);
    static void noDrag(Sprite& s, Event& e);
    MessagePtr newMessage(std::string text = "Hello World", SDL_Color color = BLACK, int ms = 2500);
protected:
    UpgradeVector mUpgrades;
private:
    bool mDragging = false;
    std::vector<MessagePtr> mMessages;
};

#endif /* SPRITE_h */
