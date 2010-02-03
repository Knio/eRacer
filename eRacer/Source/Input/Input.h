/**
 * @file Input.h
 * @brief Definition of the Input class
 *
 * @date 01.02.2010
 * @author: Ole Rehmsen
 */

#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>


#define Down(state, index)	(!!(state[index] & 0x80))
#define Up(state, index)	(!(state[index] & 0x80))


/**
 * @brief namespace for the input module
 */
namespace Input {

class Mouse;
class Keyboard;
class Gamepad;

/**
 * @brief main class of the input module. Manages devices.
 */
class Input{
public:
	/**
	 * @brief Constructor stub.
	 */
	Input();

	/**
	 * @brief initialize module. Create Devices
     *
	 * @param hWnd
	 *			a handle to the window - needed by the devices
	 * @param hInstance
	 *			a handle to the instances - needed to create DirectInput object
	 */
	void Init(HWND hWnd,HINSTANCE hInstance);

	/**
	 * @brief update all devices - this triggers polling
	 */
	void Update();
	
	/**
	 * @brief clean up devices and DirectInput object
	 */
	void Shutdown();

private:
	IDirectInput8* directInput_;
	Mouse* mouse_;
	Keyboard* keyboard_;
	Gamepad* gamepad_;

};

};

