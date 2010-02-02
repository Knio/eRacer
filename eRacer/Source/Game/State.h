#ifndef STATE_H_
#define STATE_H_

#include "Core/Time.h"

class State
{
public:
	State() {};
	virtual ~State() {};
	virtual void Tick(Time &t) { }

};


#endif