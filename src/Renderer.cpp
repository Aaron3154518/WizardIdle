#include "Renderer.h"

Renderer::Renderer() {
	for (int i = MAX_OBJS - 1; i >= 0; --i) { mStatus[i] = false; mFree.push(i); }
}

void Renderer::render(SDL_Renderer* renderer) {
	auto it = mFull.begin();
	while (it != mFull.end()) {
		if (!status[*it]) {
			it = mFull.erase(it);
			continue;
		}
		mObjs[*it].render(renderer, NULL); // Can't use boundaries with this - TODO: Add to RenderObject?
		++it;
	}
}

void Renderer::clean() {
	mFree.clear();
	mFull.clear();
	for (int i = MAX_OBJS - 1; i >= 0; --i) {
		mObjs[i].clean();
		mStatus[i] = false;
		mFree.push(i);
	}
	mCount = 0;
}

int Renderer::create(SDL_Texture* tex, Rect r) {
	if (isFull()) { return -1; }
	int idx = mFree.pop();
	mFull.push_Back(idx);
	mStatus[idx] = true;
	mObjs[idx].setTexture(tex);
	mObjs[idx].setRect(r);
	++mCount;
	return idx;
}

void Renderer::remove(int idx) {
	if (mStatus[idx]) {
		mStatus[idx] = false;
		mFree.push(idx);
		mObjs[idx].clean();
		--mCount;
	}
}

void Renderer::setTexture(int idx, SDL_Texture* tex) {
	if (mStatus[idx]) {
		mObjs[idx].setTexture(tex);
	}
}

void Renderer::setRect(int idx, Rect r) {
	if (mStatus[idx]) {
		mObjs[idx].setRect(r);
	}
}
