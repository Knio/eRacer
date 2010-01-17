#ifndef EVENT_H_
#define EVENT_H_

class Event;
class EventManager
{	
public:
	static EventManager* g_EventManager;
	EventManager() { g_EventManager = this; }
	virtual ~EventManager() { }
	virtual void Queue(Event* e) { }
	virtual void Send (Event* e) { }
	virtual void Register(int func, void* obj) { }
	
};


class Event
{
  public:
	/*
	adds this event to the global event queue. 
	it will be sent to all listeners when the event
	queue is next flushed.
	*/
	virtual void Queue() 
	{
		EventManager::g_EventManager->Queue(this);
	}

	/*
	send this event to all listners *now*.
	*/
	virtual void Send()
	{
		EventManager::g_EventManager->Send(this);
	}
};

#endif

