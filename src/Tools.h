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
    SDL_Point clickPos = { 0,0 };
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
    Value(const Number& baseVal) : m_baseVal(baseVal) {}
    ~Value() = default;

    void apply();

    const Number& getBaseValue() const { return m_baseVal; }
    const Number& getValue() const { return m_val; }

    void newAdd(const std::string& key, const Number& val) { m_add[key] = val; }
    void newMult(const std::string& key, const Number& val) { m_mult[key] = val; }
    void newPow(const std::string& key, const double& val) { m_pow[key] = val; }

private:
    Number m_baseVal, m_val;
    std::unordered_map<std::string, Number> m_add, m_mult;
    std::unordered_map<std::string, double> m_pow;
};

#endif /* TOOLS_h */
