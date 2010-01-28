#ifndef TEST_H_
#define TEST_H_

#include "Core/Event.h"
#include "Game/Module.h"
#include "Game/Game.h"

class TestModule : public Module, public Listener
{
public:

	TestModule()
	{
		// add yourself as a subsystem
		Game::GetInstance()->AddModule(this, "testmodule");

		// register for event callbacks
		REGISTER(this, KeyPressedEvent);

		// do other initialization here
		// other submodules may NOT be initialized yet, so if you depend on them, 
		// do it in Init()

	}
	~TestModule() {}

	void Init()
	{
		// do initializaton here. 
		// all other modules are guarenteed to exist at this point
		
	}
	void Start()
	{
		// main loop is about to start!
	}
	void Tick(Time& t)
	{
		// frame happened. t contains the time advancement in microseconds
	}
	void Quit()
	{
		// game stopped
	}


	// event callbacks
	int KeyPressedEvent(int key)
	{
		printf("A key was pressed in C++! (%d)\n", key);
		return 0;
	}

};


#endif
