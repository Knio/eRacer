#ifndef EVENT_H_
#define EVENT_H_

#include <assert.h>

#define EVENT(x) Event::g_Event->x
#define DEFINE_EVENT(name, ...) \
	virtual int name(__VA_ARGS__) { assert(false); return -1; } 

class Event
{	

public:
	static Event* g_Event;
	Event() { g_Event = this; }
	virtual ~Event() { }

	virtual void Register(int func, void* obj) { }
	
	DEFINE_EVENT(QuitEvent)
	DEFINE_EVENT(KeyPressedEvent,  int key)
	DEFINE_EVENT(KeyReleasedEvent, int key)
	
};



#endif

