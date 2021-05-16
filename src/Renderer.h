#ifndef RENDERER_H
#define RENDERER_H

#include <stack>
#include <vector>

#include <SDL.h>

#include "Tools/Rect.h"
#include "RenderObject.h"

constexpr int MAX_OBJS = 1024;

class Renderer {
	public:
		Renderer();
		~Renderer() { clean(); }

		void render();
		void clean();

		int create(SDL_Texture* tex, Rect r);
		void remove(int idx);
		void setTexture(int idx, SDL_Texture* tex);
		void setRect(int idx, Rect r);

		bool isFull() { return mFree.isEmpty(); }
		bool isEmpty() { return mFull.isEmpty(); }
	private:
		RenderObject mObjs[MAX_OBJS];
		bool mStatus[MAX_OBJS];
		std::stack<int> mFree;
		std::vector<int> mFull;
		int mCount = 0;
};

#endif
