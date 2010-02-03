/**
 * @file Input.cpp
 * @brief Implementation of the Input class
 *
 * @date 01.02.2010
 * @author: Ole Rehmsen
 */

#include "Input.h"

#include <cassert>

#include "Mouse.h"
#include "Keyboard.h"
#include "Gamepad.h"


namespace Input {

Input::Input()
: mouse_(NULL), keyboard_(NULL), gamepad_(NULL)
{
}

void Input::Init(HWND hWnd,HINSTANCE hInstance){
	//TODO use exceptions
	assert(SUCCEEDED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, NULL)));
	
	mouse_ = new Mouse();
	mouse_->Init(hWnd, directInput_);

	keyboard_ = new Keyboard();
	keyboard_->Init(hWnd, directInput_);
	
	gamepad_ = new Gamepad();
	gamepad_->Init(hWnd,directInput_);
}

void Input::Update(){
	mouse_->Update();
	keyboard_->Update();
	gamepad_->Update();
}

void Input::Shutdown(){
	if(NULL != mouse_){
		delete mouse_;
		mouse_ = NULL;
	}

	if(NULL != keyboard_){
		delete keyboard_;
		keyboard_ = NULL;
	}

	if(NULL != gamepad_){
		delete gamepad_;
		gamepad_ = NULL;
	}

}




};