#pragma once

#include <iostream>
#include <vector>

#include "Definitions.h"
#include "Rect.h"
#include "Number.h"
#include "Tools.h"

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
};

class WizardContext {
public:
    Crystal crystal;
    Catalyst catalyst;
    Wizard wizard;

    void init(int icon_w);
    void update(Timestep ts);
    void handleEvent(Event& e);
    void render();
private:
    std::vector<Sprite*> mSprites = { &crystal, &catalyst, &wizard };
};