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
	
	
	try { 
		Device* keyboard = new Keyboard();
		keyboard->Init(hWnd, directInput_);
		devices_.push_back(keyboard);
		
	}	catch (runtime_error) {}
	try { 
		Device* gamepad = new Gamepad();
		gamepad->Init(hWnd, directInput_);
		devices_.push_back(gamepad);
	}	catch (runtime_error) {
		if(devices_.size()<1)
			throw runtime_error("Fatal error: Neither keyboard, nor gamepad found!");
	}

	try {
		Device* mouse = new Mouse();
		mouse->Init(hWnd, directInput_);
		devices_.push_back(mouse);
	}	
	catch (runtime_error) {} //mouse is not necessary

}

void Input::Update(){
	for(vector<Device*>::iterator i = devices_.begin();
		i!= devices_.end(); i++)
		(*i)->Update();
}

void Input::Shutdown(){
	for(vector<Device*>::iterator i = devices_.begin();
		i!= devices_.end(); i++)
		delete (*i);
	devices_.clear();


}




};