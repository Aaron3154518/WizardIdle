#include "Upgrade.h"

constexpr SDL_Color bkgrnd {175,175,175};

// Upgrade
Upgrade::~Upgrade() {
    if (mDesc != nullptr) { SDL_DestroyTexture(mDesc); }
}

void Upgrade::init(int maxLevel, std::string img, std::string desc) {
    mMaxLevel = maxLevel;
    mImg = img;
    AssetManager am = Game::assets();
    mImgRect = Rect::getMinRect(am.getAsset(img), Game::icon_w, Game::icon_w);
    mDesc = am.renderTextWrapped(SMALL_FONT, desc, BLACK, 1, &mDescRect, toUint(bkgrnd));
}

void Upgrade::render(SDL_Point pos, int w) {
    mImgRect.setCenter(pos.x, pos.y);
    Game::assets().drawTexture(mImg, mImgRect, NULL);
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

void UpgradeManager::init() {
    mRect = Rect(0, 0, Game::icon_w * 6, Game::icon_w * 2);
    mTex = SDL_CreateTexture(Game::renderer(), SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, mRect.w, mRect.h);
    redraw();
}

void UpgradeManager::update(Timestep ts) {
    if (mScrollV != 0) {
        scroll(mScrollV * ts.GetSeconds());
        mScrollV *= pow(.3, ts.GetSeconds());
        if (abs(mScrollV) < 1.) { mScrollV = 0.; }
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
}

void UpgradeManager::setUpgrades(std::vector<Upgrade>* newList) {
    mUpgrades = newList;
    mScroll = mScrollMax = mScrollV = 0.;
    if (newList != nullptr) {
        mScrollMax = newList->size() * Game::icon_w;
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

    std::vector<Rect> frontRects, backRects;
    const double xRad = (double)(mRect.w - Game::icon_w) / 2.;
    const double yRad = (double)(mRect.h - Game::icon_w) / 2.;
    double w = 0., dw = 0., x = 0.;
    int sign = -1;
    bool left = true;
    auto nextX = [xRad, yRad, &w, &dw, &x, &sign] {
        double currW = w + dw * abs(x) / xRad;
        Rect r = Rect(0, 0, (int)currW, (int)currW);
        r.setCenter(x, (dw < 0 ? 1 : -1) * yRad * sqrt(1 - x*x/(xRad*xRad)));
        x += sign * currW * .6;
        x = xRad * (w + sign * 2 * x) / (dw + sign * 2 * xRad);
        if (x * dw < 0) {
            double dx = dw * sign * 2 * x / (2 * xRad + -sign * dw);
            x += dx;
        }
        return r;
    };

    Rect leftRect, middleRect, rightRect;
    do {
        w = Game::icon_w;
        dw = -w / 3.;
        x = (idx - scrollFrac) * Game::icon_w;
        middleRect = nextX();
        //if ((left && idx == 0) || (!left && idx == 10)) { continue; }
        if (left) { leftRect = nextX(); }
        else { rightRect = nextX(); }
        while(abs(x) <= abs(xRad)) { frontRects.push_back(nextX()); }
        w = Game::icon_w / 3;
        dw = w;
        x = sign * 2 * xRad - x;
        sign *= -1;
        while (sign * x <= 0) { backRects.push_back(nextX()); }
        left = !left;
    } while (!left);

    Game::setRenderTarget(mTex);
    Game::setDrawColor(bkgrnd);
    SDL_RenderFillRect(Game::renderer(), NULL);
    auto draw = [&] (Rect& r, const SDL_Color& c) {
        r.shift(mRect.w / 2, mRect.h / 2);
        Game::setDrawColor(c);
        SDL_RenderFillRect(Game::renderer(), &r);
        Game::setDrawColor(BLACK);
        SDL_RenderDrawRect(Game::renderer(), &r);
    };
    for (auto it = backRects.end(); it != backRects.begin();) { 
        --it;
        draw(*it, BLUE);
    }
    Rect r = Rect(0, 0, mRect.h * .9, mRect.h * .9);
    r.setCenter(mRect.w / 2, mRect.h / 2);
    Game::setDrawColor(GRAY);
    SDL_RenderFillRect(Game::renderer(), &r);
    for (Rect& r : frontRects) { draw(r, BLUE); } 
    draw(leftRect, GREEN);
    draw(rightRect, GREEN);
    draw(middleRect, RED);

    Game::resetRenderTarget();
}
