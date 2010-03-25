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

#include <vector>

using namespace std;

#include "Game/Module.h"
#include "Time.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Gamepad.h"

/**
 * @brief namespace for the input module
 */
namespace Input {


/**
 * @brief main class of the input module. Manages devices.
 */
class Input : public Module {
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
	void Init();


	void ReloadDevices();


	bool hasKeyboard() const;

	bool hasMouse() const;

	int getNumGamepads() const;

	/**
	 * @brief update all devices - this triggers polling
	 */
	void Tick(const Time &t);
	
	/**
	 * @brief clean up devices and DirectInput object
	 */
	void Quit();

private:
	IDirectInput8* directInput_;
	vector<Device*> devices_;

	bool hasKeyboard_;
	bool hasMouse_;
	int nGamepads_;


};

};

