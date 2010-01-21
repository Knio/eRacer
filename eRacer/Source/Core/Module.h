#ifndef MODULE_H_
#define MODULE_H_

#include "Time.h"

class Module
{

  public:
	Module() {}
	~Module() {}

	virtual void Start() {}
	virtual void Tick(Time &time) {}
	virtual void Quit() {}

};

#endif
