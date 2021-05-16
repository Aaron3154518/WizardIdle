#ifndef TOOLS_h
#define TOOLS_h

#include <unordered_map>

#include <SDL.h>

#include "Number.h"

// Helper classes
class Timestep {
public:
    Timestep(Uint32 time = 0)
        : mTime(time) {}

    operator Uint32() const { return mTime; }

    float GetSeconds() const { return mTime / 1000.0f; }
    Uint32 GetMilliseconds() const { return mTime; }
private:
    Uint32 mTime;
};

struct EventButton {
    bool pressed = false, clicked = false;
    SDL_Point clickPos = { 0,0 }, clickPosGlobal = { 0,0 };
    Uint32 duration = 0;
};

struct Event {
    bool handled = false;

    SDL_Point mouse, globalMouse;
    double mouseDx = 0., mouseDy = 0.;

    EventButton left, middle, right;

    void updateMouse(int x, int y);
    void update(Timestep ts);
    void update(SDL_Event& e);

private:
    EventButton& getButton(int sdl_button_type) {
        switch (sdl_button_type) {
        case SDL_BUTTON_LEFT:
            return left;
            break;
        case SDL_BUTTON_RIGHT:
            return right;
            break;
        case SDL_BUTTON_MIDDLE:
            return middle;
            break;
        default:
            return left;
            break;
        }
    }
};

class Value {
public:
    Value() = default;
    Value(const Number& num) : mBaseVal(num) {}
    ~Value() = default;

    void apply();

    void setBaseValue(const Number& val) { mBaseVal = val; mUpdate = true; }
    const Number& getBaseValue() const { return mBaseVal; }
    const Number& getValue() { if(mUpdate) { apply(); } return mVal; }

    operator Number() { return getValue(); }

    void newAdd(const std::string& key, const Number& val) { mAdd[key] = val; mUpdate = true; }
    void newMult(const std::string& key, const Number& val) { mMult[key] = val; mUpdate = true; }
    void newPow(const std::string& key, const double& val) { mPow[key] = val; mUpdate = true; }
private:
    bool mUpdate = true;
    Number mBaseVal, mVal;
    std::unordered_map<std::string, Number> mAdd, mMult;
    std::unordered_map<std::string, double> mPow;
};

#endif /* TOOLS_h */
