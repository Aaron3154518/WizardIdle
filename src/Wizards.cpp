#include "Wizards.h"
#include "WizardContext.h"

void drag(Sprite& s, Event& e);
void noDrag(Sprite& s, Event& e);

void drag(Sprite& s, Event& e) {
    if (e.left.clicked) {
        if (!s.mDragging && SDL_PointInRect(&e.globalMouse, &s.mRect)) {
            // Set upgrade list
            e.handled = true;
        }
        s.mDragging = false;
    }
    else if (e.left.pressed && e.left.duration >= 250 &&
        SDL_PointInRect(&e.left.clickPos, &s.mRect)) {
        s.mDragging = true;
        e.handled = true;
    }

    if (s.mDragging) {
        s.mRect.setCenter(e.globalMouse.x, e.globalMouse.y);
        e.handled = true;
    }
}
void noDrag(Sprite& s, Event& e) {
    if (e.left.clicked && SDL_PointInRect(&e.globalMouse, &s.mRect)) {
        // Set upgrade list
        e.handled = true;
    }
}

// Crystal
void Crystal::update(WizardContext& wc, Timestep ts) {
	power = (magic + 1).logBase(10) + 1;
}
void Crystal::handleEvent(WizardContext& wc, Event& e) {
	noDrag(*this, e);
}
void Crystal::render(WizardContext& wc) {
    Rect r = Game::getAbsRect(mRect);
    Game::assets().drawTexture("crystal", r, NULL);
    std::stringstream ss;
    ss << magic << "M";
    Rect textR = Rect(0, 0, Game::icon_w * 3, Game::icon_w * 1.5);
    textR.setY2(r.y);
    textR.setCenterX(r.cX());
    Game::assets().drawText(LARGE_FONT, ss.str().c_str(), BLACK, textR, NULL);
}

// Catalyst
void Catalyst::update(WizardContext& wc, Timestep ts) {
	power = (magic ^ .3) + 1;
}
void Catalyst::handleEvent(WizardContext& wc, Event& e) {
	noDrag(*this, e);
}
void Catalyst::render(WizardContext& wc) {
    Rect r = Game::getAbsRect(mRect);
    Game::assets().drawTexture("catalyst", r, NULL);
}

// Wizard
Wizard::Wizard() : mFireballs(FireballHandler(CRYSTAL)) {}
void Wizard::update(WizardContext& wc, Timestep ts) {
    power = basePower;
    power *= wc.crystal.power;

    std::vector<Fireball> vec = mFireballs.update(wc, ts);
    for (Fireball& f : vec) {
        switch (f.mTarget) {
            case CRYSTAL:
                wc.crystal.magic += f.mData;
                break;
        }
    }
    vec.clear();
    if (mFireballs.ready()) {
        mFireballs.newFireball(mRect.cX(), mRect.cY(), power);
    }
}
void Wizard::handleEvent(WizardContext& wc, Event& e) {
	drag(*this, e);
    wc.catalyst.mRect.setX2(mRect.x);
    wc.catalyst.mRect.setCenterY(mRect.cY());
}
void Wizard::render(WizardContext& wc) {
    Rect r = Game::getAbsRect(mRect);
    Game::assets().drawTexture("wizard", r, NULL);
    mFireballs.render();
}

