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
    m_val = m_baseVal;
    for (auto it = m_add.begin(); it != m_add.end(); ++it)
        m_val += it->second;
    for (auto it = m_mult.begin(); it != m_mult.end(); ++it)
        m_val *= it->second;
    for (auto it = m_pow.begin(); it != m_pow.end(); ++it)
        m_val ^= it->second;
}
