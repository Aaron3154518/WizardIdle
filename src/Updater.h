#ifndef UPDATER_H
#define UPDATER_H

#include <stack>
#include <vector>

#include <SDL.h>

#include "Tools/Rect.h"
#include "UpdateObject.h"

constexpr int MAX_OBJS = 1024;

class Updater {
	public:
		Updater();
		~Updater() { clean(); }

		void handleEvents(Event e);
		void update(TimeStep ts);
		void clean();

		int create(EventFunc eFunc, UpdateFunc uFunc);
		void remove(int idx);
		void setEventFunction(int idx, EventFunc func);
		void setUpdateFunction(int idx, UpdateFunc func);

		bool isFull() { return mFree.isEmpty(); }
		bool isEmpty() { return mFull.isEmpty(); }
	private:
		UpdateObject mObjs[MAX_OBJS];
		bool mStatus[MAX_OBJS];
		std::stack<int> mFree;
		std::vector<int> mFull;
		int mCount = 0;
};

#endif
