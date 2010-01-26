#ifndef EVENT_H_
#define EVENT_H_

/*

Event Manager
=============

Implements the Observer pattern.

Objects can register for certain event types and implement
callback methods that will be called when those events happen.

To define an event type, in the Event class below, add:

DEFINE_EVENT(name, parameter list)

for example:
DEFINE_EVENT(KeyPressedEvent, int key)

event names must end in "Event"


To send an event: anywhere, call

#include "../Core/Event.h"
...
EVENT(KeyPressedEvent(key));


To recieve an event:

1) inherit from the Listener class
2) call REGISTER(this, EventType);
3) implement the callback function for all events you are registered

ie:

class Foo : public Listener
{
public:
    Foo()
    {
        REGISTER(this, KeyPressedEvent);
    }
    virtual int KeyPressedEvent(int key)
    {
        cout << "omg, key" << key << "was pressed!\n";
        return 0;
    }
};

Event registration and dispatching is implemented in Event.py

*/


#include <assert.h>

#define EVENT(x) Event::GetInstance()->x
#define REGISTER(obj, evt) EVENT(Register(obj, #evt))

#define DEFINE_EVENT(name, ...) \
	virtual int name(__VA_ARGS__) { assert(false); return -1; } 

class Event
{	

public:
	static Event* GetInstance()
	{
		assert(g_Event);
		return g_Event;	
	}
	virtual ~Event() { }
private:
	static Event* g_Event;
protected:
	Event() { if (!g_Event) g_Event = this;	}

public:
	/* 
	The following methods are implemented in Event.py 
	If the C++ implementations get executed, it is an error
	*/
	virtual void Register(Event* obj, char* name) { assert(false); }
	/* define event types here */
	DEFINE_EVENT(QuitEvent)
	DEFINE_EVENT(KeyPressedEvent,  int key)
	DEFINE_EVENT(KeyReleasedEvent, int key)
	
};

typedef Event Listener;

#endif

