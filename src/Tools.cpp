#include "Tools.h"

void Event::update(Timestep ts) {
    for (EventButton* eb : { &left, &middle, &right }) {
        if (eb->pressed) { eb->duration += ts; }
        eb->clicked = false;
    }
}
void Event::update(SDL_Event& e) {
    switch (e.type) {
    case SDL_MOUSEBUTTONDOWN:
    {
        EventButton& eb = getButton(e.button.button);
        eb.pressed = true;
        eb.duration = 0;
        eb.clickPos = mouse;
        eb.clickPosGlobal = globalMouse;
    }
    break;
    case SDL_MOUSEBUTTONUP:
    {
        EventButton& eb = getButton(e.button.button);
        eb.pressed = false;
        eb.clicked = true;
    }
    break;
    case SDL_MOUSEMOTION:
        mouseDx = e.motion.xrel;
        mouseDy = e.motion.yrel;
        break;
    }
}

void Value::apply() {
    mVal = mBaseVal;
    for (auto it = mAdd.begin(); it != mAdd.end(); ++it)
        mVal += it->second;
    for (auto it = mMult.begin(); it != mMult.end(); ++it)
        mVal *= it->second;
    for (auto it = mPow.begin(); it != mPow.end(); ++it)
        mVal ^= it->second;
    mUpdate = false;
}
