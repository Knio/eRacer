#ifndef MODULE_H_
#define MODULE_H_

/*
Module
======

Interface for all subsystems in the game. 

*/


#include "../Core/Time.h"

class Module
{

  public:

	/** Constructor. Should initalize any values that other subsystems
		depend on (i.e, HWND and LPDIRECT3DDEVICE9 for the graphics.
		If using one of these values from another subsystem, be careful
		that it is constructed before you. */
	Module() {}
	virtual ~Module() {}
	
	/* Init - Called once after all subsystems have been constructed */
	virtual void Init() {}
	
	/* Start - Called once directly before the main loop starts */	
	virtual void Start() {}

	/* Tick - Called once per frame */
	virtual void Tick(Time &time) {}

	/* Quit - Called once directly after the main loop exits */
	virtual void Quit() {}

};

#endif

