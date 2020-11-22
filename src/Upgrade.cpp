#include "Upgrade.h"

constexpr SDL_Color bkgrnd {175,175,175,175};

// Upgrade
Upgrade::~Upgrade() {
    if (mDesc != nullptr) { SDL_DestroyTexture(mDesc); }
}

void Upgrade::init(int maxLevel, std::string img, std::string desc) {
    mMaxLevel = maxLevel;
    mImg = img;
    mDesc = Game::assets().renderTextWrapped(SMALL_FONT, desc,
            BLACK, Game::icon_w * 2, &mDescRect, toUint(bkgrnd));
}

void Upgrade::render(Rect& r) {
    SDL_Texture* tex = Game::assets().getAsset(mImg);
    double cx = r.cX(), cy = r.cY();
    r = Rect::getMinRect(tex, r.w, r.h);
    r.setCenter(cx, cy);
    Game::assets().drawTexture(tex, r, NULL);
}

void Upgrade::renderDescription(SDL_Point pos) {
    if (pos.x < Game::screen().w / 2) {
        mDescRect.x = pos.x;
    } else {
        mDescRect.setX2(pos.x);
    }
    if (pos.y < Game::screen().h / 2) {
        mDescRect.y = pos.y;
    } else {
        mDescRect.setY2(pos.y);
    }
    Game::assets().drawTexture(mDesc, mDescRect, NULL);
}

// Upgrade Manager
UpgradeManager::~UpgradeManager() {
    if (mTex != nullptr) { SDL_DestroyTexture(mTex); }
}

static Upgrade u;
void UpgradeManager::init() {
    mRect = Rect(0, 0, Game::icon_w * 6, Game::icon_w * 2);
    mTex = SDL_CreateTexture(Game::renderer(), SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, mRect.w, mRect.h);
  //  redraw();
    mUpgrades = new std::vector<Upgrade*>();
    u.init(1, "fireball", "hello world");
    for (int i = 0; i < 10; ++i) { mUpgrades->push_back(&u); }
    setUpgrades(mUpgrades);

}

void UpgradeManager::update(Timestep ts) {
    if (mScrollV != 0) {
        scroll(mScrollV * ts.GetSeconds());
        mScrollV *= pow(.3, ts.GetSeconds());
        if (mScroll == 0 || mScroll == mScrollMax ||
                abs(mScrollV) < 1.) { mScrollV = 0.; }
    } 
}

void UpgradeManager::handleEvent(Event& e) {
    if (mDragging) {
        scroll(-e.mouseDx);
        if (e.left.clicked) {
            mScrollV = -e.mouseDx / Game::ts().GetSeconds();
            mDragging = false;
        }
        e.handled = true;
    } else if (e.left.pressed && SDL_PointInRect(&e.mouse, &mRect)) {
        mScrollV = 0.;
        mDragging = e.handled = true;
    }
}

void UpgradeManager::render() {
    mRect.setCenterX(Game::screen().cX());
    mRect.y = 0;
    Game::assets().drawTexture(mTex, mRect, NULL);
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);
    SDL_Point localMouse = { mouse.x - mRect.x, mouse.y - mRect.y };
    for (auto it = mURects.begin(); it != mURects.end(); ++it) {
        if (SDL_PointInRect(&localMouse, &it->second)) {
            mUpgrades->at(it->first)->renderDescription(mouse);
            break;
        }
    }
}

void UpgradeManager::setUpgrades(std::vector<Upgrade*>* newList) {
    mUpgrades = newList;
    mScroll = mScrollMax = mScrollV = 0.;
    if (newList != nullptr) {
        mScrollMax = (newList->size() - 1) * Game::icon_w;
    }
    redraw();
}

void UpgradeManager::scroll(double ds) {
    double oldScroll = mScroll;
    mScroll = std::min(std::max(mScroll + ds, 0.), mScrollMax);
    if (mScroll != oldScroll) { redraw(); }
}

void UpgradeManager::redraw() {
    double scrollFrac = mScroll / Game::icon_w;
    int idx = (int)(scrollFrac + .5);

    std::vector<Rect> backRects;
    mURects.clear();
    const double xRad = (double)(mRect.w - Game::icon_w) / 2.;
    const double yRad = (double)(mRect.h - Game::icon_w) / 2.;
    double w = 0., dw = 0., x = 0.;
    int sign = -1;
    bool left = true, front = true;
    auto nextX = [xRad, yRad, &w, &dw, &x, &sign, &front] {
        double currW = w + dw * abs(x) / xRad;
        Rect r = Rect(0, 0, (int)currW, (int)currW);
        r.setCenter(x, (front ? 1 : -1) * yRad * sqrt(1 - x*x/(xRad*xRad)));
        x += sign * currW * (front  ? .6 : .55);
        x = xRad * (2 * x + sign * w) / (2 * xRad - sign * dw);
        if (x < 0) {
            double dx = -2 * dw * abs(x) / (sign * 2 * xRad + dw);
            x -= dx;
        }
        return r;
    };

    int l = mUpgrades == nullptr ? 0 : mUpgrades->size();
    do {
        int i = idx;
        front = true;
        w = Game::icon_w;
        dw = -w / 3.;
        x = (idx - scrollFrac) * Game::icon_w;
        while(abs(x) <= abs(xRad) & 0 <= i && i < l) {
            mURects[i] = nextX();
            i += sign;
        }
        w /= 3.;
        dw = w;
        x = sign * 2 * xRad - x;
        sign *= -1;
        front = false;
        while (sign * x <= 0 && 0 <= i && i < l) {
            backRects.push_back(nextX());
            i -= sign;
        }
        left = !left;
    } while (!left);

    Game::setRenderTarget(mTex);
    Game::setDrawColor(bkgrnd);
    SDL_RenderFillRect(Game::renderer(), NULL);
    double cx = mRect.w / 2, cy = mRect.h / 2;
    for (Rect& r : backRects) {
        r.shift(cx, cy);
        Game::setDrawColor(BLUE);
        SDL_RenderFillRect(Game::renderer(), &r);
        Game::setDrawColor(BLACK);
        SDL_RenderDrawRect(Game::renderer(), &r);
    }
    Rect r = Rect(0, 0, mRect.h * .9, mRect.h * .9);
    r.setCenter(cx, cy);
    Game::setDrawColor(GRAY);
    SDL_RenderFillRect(Game::renderer(), &r);
    for (auto it = mURects.begin(); it != mURects.end(); ++it) {
        it->second.shift(cx, cy);
        mUpgrades->at(it->first)->render(it->second);
    }

    Game::resetRenderTarget();
}
