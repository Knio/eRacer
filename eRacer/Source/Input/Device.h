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
	 * @brief Destructor stub.
	 */
	virtual ~Device();

	/**
	 * @brief initialize a devices. Should be overriden by the subclasses
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
	 * @brief poll the state of the device and emit events
	 *
	 * Should be overriden by base classes. Can only be called once
	 */
	virtual void Update();

	/**
	 * @brief Clean up, release device
	 */
	virtual void Shutdown();

	IDirectInputDevice8* m_pDevice;

protected:
	bool initialized_;
	bool m_BufferFlip;



	void flipBuffers(){ m_BufferFlip = !m_BufferFlip; }
	void handleCreateDeviceReturnCode(HRESULT returnCode);
};






};