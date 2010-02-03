/**
 * @file Input.cpp
 * @brief Implementation of the Input class
 *
 * @date 01.02.2010
 * @author: Ole Rehmsen
 */

#include "Input.h"

#include <cassert>
#include <stdexcept>

using namespace std;

namespace Input {

Input::Input()
{
}

void Input::Init(HWND hWnd,HINSTANCE hInstance){
	HRESULT hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, NULL);
	
	assert(DIERR_BETADIRECTINPUTVERSION != hr);
	assert(DIERR_INVALIDPARAM != hr);
	assert(DIERR_OLDDIRECTINPUTVERSION != hr);

	if(DIERR_OUTOFMEMORY == hr)
		throw runtime_error("Could not create DirectInput object - out of memory!");

	mouse.Init(hWnd, directInput_);
	keyboard.Init(hWnd, directInput_);
	gamepad.Init(hWnd, directInput_);
}

void Input::Update(){
	mouse.Update();
	keyboard.Update();
	gamepad.Update();
}

void Input::Shutdown(){
}




};