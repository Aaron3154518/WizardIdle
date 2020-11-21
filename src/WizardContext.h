#ifndef WIZARDCONTEXT_h
#define WIZARDCONTEXT_h

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "Definitions.h"
#include "Rect.h"
#include "Number.h"
#include "Tools.h"

constexpr int CRYSTAL = 0;
constexpr int CATALYST = 1;
constexpr int WIZARD = 2;

class WizardContext;

class Sprite {
public:
    bool mVisible = false, mDragging = false;
    Rect mRect;

    Sprite() = default;
    ~Sprite() = default;

    virtual void update(WizardContext& wc, Timestep ts) {}
    virtual void handleEvent(WizardContext& wc, Event& e) {}
    virtual void render(WizardContext& wc) {}
};

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

class Crystal : public Sprite {
public:
    Number magic, power;

    Crystal() = default;
    ~Crystal() = default;

    void update(WizardContext& wc, Timestep ts);
    void handleEvent(WizardContext& wc, Event& e);
    void render(WizardContext& wc);
};
class Catalyst : public Sprite {
public:
    Number capacity, power;

    Catalyst() = default;
    ~Catalyst() = default;

    void update(WizardContext& wc, Timestep ts);
    void handleEvent(WizardContext& wc, Event& e);
    void render(WizardContext& wc);

    void addMagic(Number& add) {
        magic = Number::getMin(capacity, magic + add);
    }
    const Number& getMagic() { return magic; }
private:
    Number magic;
};
class Wizard : public Sprite {
public:
    Number power;

    Wizard() = default;
    ~Wizard() = default;

    void update(WizardContext& wc, Timestep ts);
    void handleEvent(WizardContext& wc, Event& e);
    void render(WizardContext& wc);
private:
    FireballHandler mFireballs = FireballHandler(CRYSTAL);
};

class WizardContext {
public:
    Crystal crystal;
    Catalyst catalyst;
    Wizard wizard;

    WizardContext() = default;
    ~WizardContext() = default;

    void init();
    void update(Timestep ts);
    void handleEvent(Event& e);
    void render();
    Sprite& getSprite(int id);
private:
    std::vector<Sprite*> mSprites = { &crystal, &catalyst, &wizard };
};

#endif /* WIZARDCONTEXT_h */
