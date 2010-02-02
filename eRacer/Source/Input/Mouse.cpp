/**
 * @file Mouse.cpp
 * @brief Implementation of the Mouse class
 *
 * @date 01.02.2010
 * @author: Ole Rehmsen
 */

#include "Mouse.h"

#include "Core/Event.h"

#include <iostream>
using namespace std;

namespace Input {

void Mouse::Init(HWND hWnd, IDirectInput* directInput)
{
	//for now, make sure this can only be called once
	assert(NULL == m_pDevice);

	m_BufferFlip = false;

	DIDEVCAPS mouseCapabilities; 

	assert(SUCCEEDED(directInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)));
	assert(SUCCEEDED(m_pDevice->SetDataFormat(&c_dfDIMouse2)));
	assert(SUCCEEDED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)));
	
	//if this fails, it will be acquired in the keyboard update function
	//this will lead to the mouse state not being initialized, and may lead to problems
	if(SUCCEEDED(m_pDevice->Acquire()))
		m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), (LPVOID)&currentState());
	else
		memset(&currentState(),0,sizeof(DIMOUSESTATE2));

	mouseCapabilities.dwSize = sizeof(mouseCapabilities);
	m_pDevice->GetCapabilities(&mouseCapabilities);

	assert(mouseCapabilities.dwFlags & DIDC_ATTACHED);

	//m_dwNAxes = mouseCapabilities.dwAxes;
	//m_dwNButtons = mouseCapabilities.dwButtons;

	flipBuffers();

}


HRESULT Mouse::Update(void)
{
	HRESULT hr = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), (LPVOID)&currentState());
	
	switch(hr){
		case DI_OK:
			break; //everything is fine
		case DIERR_INPUTLOST:  
		case DIERR_NOTACQUIRED:
			m_pDevice->Acquire(); //get the device back 
			return hr;
		case E_PENDING: //not ready yet, maybe next frame
			return hr;
		default:
			assert(hr != DIERR_NOTINITIALIZED);
	}

	/* emit events */
	
	if(currentState().lX || currentState().lY)
		EVENT(MouseMovedEvent(currentState().lX,currentState().lY));

	if(currentState().lZ)
		EVENT(MouseWheelEvent(currentState().lY));

	for(int i=0; i<N_MOUSE_BUTTONS; i++)
	{
		if (!KeyDown(currentState().rgbButtons, i) && KeyDown(oldState().rgbButtons, i))
			EVENT(MouseButtonReleasedEvent(i));
		else if (KeyDown(currentState().rgbButtons, i) && !KeyDown(oldState().rgbButtons, i))
			EVENT(MouseButtonPressedEvent(i));
	}

	flipBuffers();

	return DI_OK; 
}

void Mouse::Shutdown(void)
{
	if(NULL != m_pDevice)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}
}



bool Mouse::isButtonDown(int button)
{
	return KeyDown(oldState().rgbButtons,button);
} 

}