#include "WizardContext.h"
#include "Game.h"
#include "AssetManager.h"

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

void Crystal::update(WizardContext& wc, Timestep ts) {
	power = (magic + 1).logBase(10) + 1;
}
void Crystal::handleEvent(WizardContext& wc, Event& e) {
	noDrag(*this, e);
}
void Crystal::render(WizardContext& wc) {
    Game g;
    Rect r = g.getAbsRect(mRect);
	g.assets().drawTexture("crystal", r, NULL);
}

void Catalyst::update(WizardContext& wc, Timestep ts) {
	power = (magic ^ .3) + 1;
}
void Catalyst::handleEvent(WizardContext& wc, Event& e) {
	noDrag(*this, e);
}
void Catalyst::render(WizardContext& wc) {
    Game g;
    Rect r = g.getAbsRect(mRect);
    std::cout << r << std::endl;
    g.assets().drawTexture("catalyst", r, NULL);
}

void Wizard::update(WizardContext& wc, Timestep ts) {}
void Wizard::handleEvent(WizardContext& wc, Event& e) {
	drag(*this, e);
    wc.catalyst.mRect.setX2(mRect.x);
    wc.catalyst.mRect.setCenterY(mRect.cY());
}
void Wizard::render(WizardContext& wc) {
    Game g;
    Rect r = g.getAbsRect(mRect);
    g.assets().drawTexture("wizard", r, NULL);
}

void WizardContext::init(int icon_w) {
    AssetManager am = Game().assets();
    crystal.mRect = Rect::getMinRect(am.getAsset("crystal"), icon_w * 3 / 2, icon_w * 3 / 2);
    crystal.mRect.setCenter(0, 0);
    catalyst.mRect = Rect::getMinRect(am.getAsset("catalyst"), icon_w / 2, icon_w / 2);
    wizard.mRect = Rect::getMinRect(am.getAsset("wizard"), icon_w, icon_w);
    wizard.mRect.setCenter(0, 0);

    crystal.mVisible = true;
    wizard.mVisible = true;
}
void WizardContext::update(Timestep ts) {
    // Custom update order? (or just custom update no function calls)
    for (Sprite* s : mSprites) {
        if (s->mVisible) { s->update(*this, ts); }
    }
}
void WizardContext::handleEvent(Event& e) {
    for (auto it = mSprites.end(); it != mSprites.begin();) {
        --it;
        if ((*it)->mVisible) { (*it)->handleEvent(*this, e); }
    }
}
void WizardContext::render() {
    for (Sprite* s : mSprites) {
        if (s->mVisible) { s->render(*this); }
    }
}