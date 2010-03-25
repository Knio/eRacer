#ifndef EVENT_H_
#define EVENT_H_

/*

Event Manager
=============

Implements the Observer pattern.

Objects can register for certain event types and implement
callback methods that will be called when those events happen.

To define an event type*, in the Event class below, add:

DEFINE_EVENT(name, parameter list)

for example:
DEFINE_EVENT(KeyPressedEvent, int key)

event names must end in "Event"
(*Events that are used purely in python do
 not need to be defined)


To send an event: anywhere, call

#include "Core/Event.h"
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

If you get an error in this file, it is because:

1) you REGISTERED for an event, but did not implement a callback
2) you REGISTERED or sent an EVENT without DEFINE_EVENTing it
3) you called REGISTER or EVENT before the Event singleton was initialized.

*/


//#include <NxUserContactReport.h> 
#include <assert.h>
#include "Game/State.h"
#include "Core/Math.h"

class NxContactPair;

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

	DEFINE_EVENT(MouseButtonPressedEvent,  int mouseButton)
	DEFINE_EVENT(MouseButtonReleasedEvent, int mouseButton)
	DEFINE_EVENT(MouseMovedEvent, long relativeX, long relativeY)
	DEFINE_EVENT(MouseWheelEvent, long relativeWheel)

	DEFINE_EVENT(GamepadButtonPressedEvent, int gamepadId, int gamepadButton)
	DEFINE_EVENT(GamepadButtonReleasedEvent, int gamepadId, int gamepadButton)
	DEFINE_EVENT(GamepadStick1AbsoluteEvent, int gamepadId, long x, long y)
	DEFINE_EVENT(GamepadStick2AbsoluteEvent, int gamepadId, long x, long y)
	DEFINE_EVENT(GamepadStick1RelativeEvent, int gamepadId, long x, long y)
	DEFINE_EVENT(GamepadStick2RelativeEvent, int gamepadId, long x, long y)
	DEFINE_EVENT(GamepadTriggerAbsoluteEvent, int gamepadId, long z)
	DEFINE_EVENT(GamepadTriggerRelativeEvent, int gamepadId, long z)


	DEFINE_EVENT(ReloadConstsEvent)
	DEFINE_EVENT(ReloadedConstsEvent)

	DEFINE_EVENT(GameStateChangeEvent, State* state)
	
	DEFINE_EVENT(MeteorMeteorCollisionEvent, 	int meteorId1, 	int meteorId2, 	Vector3 force)
	DEFINE_EVENT(MeteorTrackCollisionEvent, 	int meteorId, 	int trackId, 	Vector3 force)
	DEFINE_EVENT(MeteorCarCollisionEvent, 		int meteorId, 	int carId, 		Vector3 force)
	DEFINE_EVENT(CarTrackCollisionEvent,		int carId, 		int trackId, 	Vector3 force)
	DEFINE_EVENT(ObstacleAheadEvent,			int carId, 		int obstacleId)
};

typedef Event Listener;

#endif

