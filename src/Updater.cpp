#include "Updater.h"

Updater::Updater() {
	for (int i = MAX_OBJS - 1; i >= 0; --i) { mStatus[i] = false; mFree.push(i); }
}

void Updater::handleEvents(Event& e) {
	auto it = mFull.begin();
	while (it != mFull.end()) {
		if (!status[*it]) {
			it = mFull.erase(it);
			continue;
		}
		mObjs[*it].handleEvent(e);
		++it;
	}
}

void Updater::update(Timestep& ts) {
	auto it = mFull.begin();
	while (it != mFull.end()) {
		if (!status[*it]) {
			it = mFull.erase(it);
			continue;
		}
		mObjs[*it].update(ts);
		++it;
	}
}

void Updater::clean() {
	mFree.clear();
	mFull.clear();
	for (int i = MAX_OBJS - 1; i >= 0; --i) {
		mStatus[i] = false;
		mFree.push(i);
	}
	mCount = 0;
}

int Updater::create(EventFunc eFunc, UpdateFunc uFunc) {
	if (isFull()) { return -1; }
	int idx = mFree.pop();
	mFull.push_Back(idx);
	mStatus[idx] = true;
	mObjs[idx].setEventFunction(eFunc);
	mObjs[idx].setUpdateFunction(uFunc);
	++mCount;
	return idx;
}

void Updater::remove(int idx) {
	if (mStatus[idx]) {
		mStatus[idx] = false;
		mFree.push(idx);
		--mCount;
	}
}

void Updater::setEventFunction(int idx, EventFunc func) {
	if (mStatus[idx]) {
		mObjs[idx].setEventFunction(func);
	}
}

void Updater::setUpdateFunction(int idx, UpdateFunc func) {
	if (mStatus[idx]) {
		mObjs[idx].setUpdateFunction(func);
	}
}
