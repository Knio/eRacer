#ifndef TEST_H_
#define TEST_H_

#include "Core/Event.h"
#include "Game/Module.h"
#include "Game/Game.h"
#include "Graphics/GraphicsLayer.h"

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


		// random shit
		/*
		Matrix m;
		D3DXMatrixTranslation(&m, 10, 1, -3);
		debug(m);
		debug(CreateMatrix(Vector3(10,1,-3)));
		*/


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
		Sound::SoundFx sound;
		sound.isPaused 	= FALSE;
		sound.is3D 			= FALSE;
		Sound::Sound()->LoadSoundFx("EngineSound.wav", &sound);
		
	}
	void Tick(Time& t)
	{
		
/*
		Graphics::GraphicsModule()->WriteString("Hello, World", "MS Gothic", 25, Vector3(300, 300, 0), Vector3(1.0f, 0.0f, 1.0f));
		Graphics::GraphicsModule()->WriteString("I'm small", "MS Gothic", 15, Vector3(100, 100, 0), Vector3(1.0f, 1.0f, 1.0f));
		Graphics::GraphicsModule()->WriteString("Courier font", "Courier", 55, Vector3(100, 500, 0), Vector3(0.5f, 0.7f, 0.9f));
*/		
		// frame happened. t contains the time advancement in microseconds
	}
	void Quit()
	{
		// game stopped
	}


	// event callbacks
	int KeyPressedEvent(int key)
	{
		//printf("A key was pressed in C++! (%d)\n", key);
		return 0;
	}

};


#endif
