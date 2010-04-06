/**
 * @file Input.cpp
 * @brief Implementation of the Input class
 *
 * @date 01.02.2010
 * @author: Ole Rehmsen
 */

#include "Input.h"
#include "Game/Game.h"

#include <cassert>
#include <stdexcept>

using namespace std;

namespace Input {

Input::Input()
{
}

void Input::Init(){
	HRESULT hr = DirectInput8Create(Game::GetInstance()->hinst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, NULL);
	
	assert(DIERR_BETADIRECTINPUTVERSION != hr);
	assert(DIERR_INVALIDPARAM != hr);
	assert(DIERR_OLDDIRECTINPUTVERSION != hr);

	if(DIERR_OUTOFMEMORY == hr)
		throw runtime_error("Could not create DirectInput object - out of memory!");
	
	
	ReloadDevices();

}

void Input::ReloadDevices(){
	try { 
		Device* keyboard = new Keyboard();
		keyboard->Init(Game::GetInstance()->hwnd, directInput_);
		devices_.push_back(keyboard);
		hasKeyboard_ = true;	
	}	catch (runtime_error) {
		hasKeyboard_ = false;
	}
	try { 
		nGamepads_ = 0;
		while(nGamepads_<4){
			Gamepad* gamepad = new Gamepad(nGamepads_);
			gamepad->Init(Game::GetInstance()->hwnd, directInput_);
			devices_.push_back(gamepad);
			nGamepads_++;
		}
	}	catch (runtime_error) {
		if(devices_.size()<1)
			throw runtime_error("Fatal error: Neither keyboard, nor gamepad found!");
	}

	try {
		Device* mouse = new Mouse();
		mouse->Init(Game::GetInstance()->hwnd, directInput_);
		devices_.push_back(mouse);
		hasMouse_ = true;
	}	
	catch (runtime_error) {
		hasMouse_ = false;
	}
}


bool Input::HasKeyboard() const{
	return hasKeyboard_;
}

bool Input::HasMouse() const{
	return hasMouse_;
}

int Input::GetNumGamepads() const{
	return nGamepads_;
}


void Input::Tick(const Time &t){
	for(vector<Device*>::iterator i = devices_.begin();
		i!= devices_.end(); i++)
		(*i)->Update();
}

void Input::Quit(){
	for(vector<Device*>::iterator i = devices_.begin();
		i!= devices_.end(); i++)
		delete (*i);
	devices_.clear();


}




};