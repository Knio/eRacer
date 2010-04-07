#ifndef GAME_H_
#define GAME_H_

#include <assert.h>
#include "Module.h"


/*
Game
====

Main game class. Root of the game engine.
Contains all subsystems and main loop.

This class mostly implemented in Game.py
and in Main.py

Do not subclass Game.

*/
namespace Graphics {
class Window;
}

class Game
{
public:
	static Game* GetInstance() { 
		assert(g_Game);
		return g_Game; 
	}
	HWND 					hwnd;
	HINSTANCE			hinst;
	bool 					debug;	
	Graphics::Window* 	window; 
private:
	static Game* g_Game;
protected:
	Game() { g_Game = this; debug=FALSE; }
	
public:
	virtual ~Game() {}
	
	/* Get a named module. */
	virtual Module* GetModule(char* name)					{ assert(false); return NULL; }
	
	/* Add a module to the game, with an optional name */
	virtual void AddModule(Module* module, const char* name=NULL) { assert(false); }

protected:
	/* Initialize subsystems. Calls Module::Init on each module. See Module.h */	
	virtual void Init()										{ assert(false); }

	/* Run the main loop until exit. */
	virtual void Run()										{ assert(false); }

	/* Start the main loop. Calls Module::Start on each module. See Module.h */	
	virtual void Start()									{ assert(false); }

	/* Do one frame. Calls Module::Tick on each module. See Module.h */	
	virtual void Start(Time& t)						{ assert(false); }

	/* Quit the main loop. Calls Module::Quit on each module. See Module.h */	
	virtual void Quit()										{ assert(false); }

};

#endif
