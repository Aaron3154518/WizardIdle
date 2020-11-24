#include "Upgrade.h"
#include "WizardContext.h"

constexpr SDL_Color bkgrnd {175,175,175,175};

// Upgrade
Upgrade::~Upgrade() {
    if (mDesc != nullptr) { SDL_DestroyTexture(mDesc); }
}

void Upgrade::init(int maxLevel, std::string img, std::string desc) {
    mMaxLevel = maxLevel;
    mImg = img;
    infoText.fontId = SMALL_FONT; infoText.text = desc;
    infoText.xMode = CENTER; infoText.yMode = TOPLEFT;
    infoText.w = Game::icon_w * 3;
    mDesc = Game::assets().renderTextWrapped(infoText, mDescRect, toUint(bkgrnd));
}

void Upgrade::render(Rect& r) {
    SDL_Texture* tex = Game::assets().getAsset(mImg);
    Rect imgR = Rect::getMinRect(tex, r.w, r.h);
    imgR.setCenter(r.cX(), r.cY());
    Game::assets().drawTexture(tex, imgR, NULL);
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
    infoText.x = mDescRect.cX(); infoText.y = mDescRect.y2();
    infoText.text = getInfo();
    Game::assets().drawTextWrapped(infoText, NULL, toUint(bkgrnd));
}

// Upgrade Manager
UpgradeManager::~UpgradeManager() {
    if (mTex != nullptr) { SDL_DestroyTexture(mTex); }
}

void UpgradeManager::init() {
    mRect = Rect(0, 0, Game::icon_w * 6, Game::icon_w * 2);
    mTex = SDL_CreateTexture(Game::renderer(), SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, mRect.w, mRect.h);
    setSprite(CRYSTAL);
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
    } else if (SDL_PointInRect(&e.left.clickPos, &mRect)) {
        auto upgrades = getUpgrades();
        if (e.left.clicked) {
            SDL_Point pos = { e.mouse.x - mRect.x, e.mouse.y - mRect.y };
            for (auto it = mFrontRects.begin(); it != mFrontRects.end(); ++it) {
                if (SDL_PointInRect(&pos, &it->second)) {
                    Upgrade* u = upgrades.at(it->first);
                    if (u->canBuy()) {
                        u->levelUp();
                    }
                }
            }
            e.handled = true;
        } else if (e.left.pressed) {
            mScrollV = 0.;
            if (e.mouseDx != 0 || e.mouseDy != 0) {
                mDragging = true;
            }
            e.handled = true;
        }
    }
}

void UpgradeManager::render() {
    mRect.y = 0;
    mRect.setCenterX(Game::screen().w / 2);
    Game::assets().drawTexture(mTex, mRect, NULL);
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);
    SDL_Point localMouse = { mouse.x - mRect.x, mouse.y - mRect.y };
    auto upgrades = getUpgrades();
    for (Upgrade* u : upgrades) { if (u->updateMe) { redraw(); break; } }
    Game::setRenderTarget(mTex);
    for (auto it = mFrontRects.begin(); it != mFrontRects.end(); ++it) {
        Upgrade* u = upgrades.at(it->first);
        Game::setDrawColor(u->maxLevel() ? YELLOW : u->canBuy() ? GREEN : RED);
        SDL_RenderDrawRect(Game::renderer(), &it->second);
    }
    Game::resetRenderTarget();
    for (auto it = mFrontRects.begin(); it != mFrontRects.end(); ++it) {
        if (SDL_PointInRect(&localMouse, &it->second)) {
            upgrades.at(it->first)->renderDescription(mouse);
            break;
        }
    }
}

void UpgradeManager::setSprite(int id) {
    mSelected = id;
    mSize = Wizards::getSprite(id).getUpgrades().size();
    mScroll = mScrollV = 0.;
    mScrollMax = (mSize - 1) * Game::icon_w;
    calcRects();
}

void UpgradeManager::scroll(double ds) {
    double oldScroll = mScroll;
    mScroll = std::min(std::max(mScroll + ds, 0.), mScrollMax);
    if (mScroll != oldScroll) { calcRects(); }
}

std::vector<Upgrade*> UpgradeManager::getUpgrades() {
    std::vector<Upgrade*> upgrades = Wizards::getSprite(mSelected).getUpgrades();
    int size = upgrades.size();
    if (mSize != size) {
        mSize = size;
        mScrollMax = (mSize - 1) * Game::icon_w;
        if (mScroll > mScrollMax) { mScroll = mScrollMax; }
        calcRects();
    }
    return upgrades;
}

void UpgradeManager::redraw() {
    Game::setRenderTarget(mTex);
    Game::setDrawColor(bkgrnd);
    SDL_RenderFillRect(Game::renderer(), NULL);
    for (auto it = mBackRects.begin(); it != mBackRects.end(); ++it) {
        Game::setDrawColor(BLUE);
        SDL_RenderFillRect(Game::renderer(), &it->second);
        Game::setDrawColor(BLACK);
        SDL_RenderDrawRect(Game::renderer(), &it->second);
    }
    Sprite& s = Wizards::getSprite(mSelected);
    SDL_Texture* tex = Game::assets().getAsset(s.getImage());
    Rect r = Rect::getMinRect(tex, mRect.h * .9, mRect.h * .9);
    r.setCenter(mRect.w / 2, mRect.h / 2);
    SDL_RenderCopy(Game::renderer(), tex, NULL, &r);
    auto upgrades = s.getUpgrades();
    for (auto it = mFrontRects.begin(); it != mFrontRects.end(); ++it) {
        upgrades.at(it->first)->render(it->second);
    }
    Game::resetRenderTarget();

    for (Upgrade* u : upgrades) { u->updateMe = false; }
}

void UpgradeManager::calcRects() {
    double scrollFrac = mScroll / Game::icon_w;
    int idx = (int)(scrollFrac + .5);

    mFrontRects.clear();
    mBackRects.clear();
    const double xRad = (double)(mRect.w - Game::icon_w) / 2.;
    const double yRad = (double)(mRect.h - Game::icon_w) / 2.;
    double cx = mRect.w / 2, cy = mRect.h / 2;
    double w = 0., dw = 0., x = 0.;
    int sign = -1;
    bool left = true, front = true;
    auto nextX = [xRad, yRad, cx, cy, &w, &dw, &x, &sign, &front] {
        double currW = w + dw * abs(x) / xRad;
        Rect r = Rect(0, 0, (int)currW, (int)currW);
        r.setCenter(x, (front ? 1 : -1) * yRad * sqrt(1 - x*x/(xRad*xRad)));
        r.shift(cx, cy);
        x += sign * currW * (front  ? .6 : .55);
        x = xRad * (2 * x + sign * w) / (2 * xRad - sign * dw);
        if (x < 0) {
            double dx = -2 * dw * abs(x) / (sign * 2 * xRad + dw);
            x -= dx;
        }
        return r;
    };

    do {
        int i = idx;
        front = true;
        w = Game::icon_w;
        dw = -w / 3.;
        x = (idx - scrollFrac) * Game::icon_w;
        while(abs(x) <= abs(xRad) & 0 <= i && i < mSize) {
            mFrontRects[i] = nextX();
            i += sign;
        }
        w /= 3.;
        dw = w;
        x = sign * 2 * xRad - x;
        sign *= -1;
        front = false;
        while (sign * x <= 0 && 0 <= i && i < mSize) {
            mBackRects[i] = nextX();
            i -= sign;
        }
        left = !left;
    } while (!left);

    redraw();
}
