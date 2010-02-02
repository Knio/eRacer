

#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>


//TODO rename
#define KeyDown(data, n)	(!!(data[n] & 0x80))
#define KeyUp(data, n)		(!(data[n] & 0x80))



namespace Input {

class Mouse;
class Keyboard;

class Input{
public:
	Input();

	void Init(HWND hWnd,HINSTANCE hInstance);
	void Update();
	void Shutdown();

private:
	IDirectInput* directInput_;
	Mouse* mouse_;
	Keyboard* keyboard_;

};

};