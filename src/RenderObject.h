#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include <SDL.h>

#include "Tools/Rect.h"

class RenderObject {
	public:
		RenderObject() = default;
		RenderObject(Rect r, SDL_Texture* tex) : mRect(r), mTex(tex) {}
		~RenderObject() { clean(); }

		void setRect(Rect r) { mRect = r; }
		Rect getRect() { return mRect; }
		void setT_Hexture(SDL_Texture* tex);

		void render(SDL_Renderer* renderer, Rect* boundary);
		void clean();
	private:
		Rect mRect;
		SDL_Texture* mTex = nullptr;
};

#endif
