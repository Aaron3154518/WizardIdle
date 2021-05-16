#ifndef UPDATE_OBJECT_H
#define UPDATE_OBJECT_H

#include "Tools/Tools.h"
#include "Tools/Rect.h"

// TODO: typedefs
typedef EventFunc
typedef UpdateFunc

class UpdateObject {
	public:
		UpdateObject() = default;
		~UpdateObject() = default;

		void handleEvents(Event& e) { mEventFunc(e) }
		void update(TimeStep& ts) { mUpdateFunc(ts) }

		void setEventFunction(EventFunc func) { mEventFunc = func; }
		void setUpdateFunction(UpdateFunc func) { mUpdateFunc = func; }
	private:
		EventFunc mEventFunc;
		UpdateFunc mUpdateFunc;
};

#endif
