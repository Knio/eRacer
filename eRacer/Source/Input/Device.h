/**
 * @file Device.h
 * @brief Definition of the Device class
 *
 * @date 03.02.2010
 * @author: Ole Rehmsen
 */

#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <cassert>
#include <iostream>
#include <stdexcept>
using namespace std;

namespace Input {

#define Down(state, index)	(!!(state[index] & 0x80))
#define Up(state, index)	(!(state[index] & 0x80))

/**
 * @brief Base class for all device wrappers
 */
class Device
{
public:
	/**
	 * @brief Constructor stub.
	 */
	Device();

	/**
	 * @brief Destructor - release devices and DirectInput
	 */
	virtual ~Device();

	/**
	 * @brief initialize a mouse
	 *
	 * @param hWnd 
	 *			a handle to the window
	 * @param directInput 
	 *			a pointer to the DirectInput interface
	 * @throws runtime_error 
	 *			if the mouse is not registered, not attached or we are out of memory
	 */
	virtual void Init(HWND hWnd, IDirectInput8* directInput);

	/**
	 * @brief poll the state of of the mouse and emit events
	 */
	virtual void Update();

	/**
	 * @brief Clean up
	 */
	virtual void Shutdown();

protected:
	bool initialized_;
	bool m_BufferFlip;

	IDirectInputDevice8* m_pDevice;


	void flipBuffers(){ m_BufferFlip = !m_BufferFlip; }
};






};