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
#include <stdexcept>
using namespace std;

namespace Input {

Mouse::Mouse()	
: m_pDevice(NULL),
  m_BufferFlip(false),
  initialized_(false){ 

}

Mouse::~Mouse() { 
	Shutdown(); 
}



void Mouse::Init(HWND hWnd, IDirectInput8* directInput)
{
	//for now, make sure this can  only be called once
	assert(!initialized_);

	HRESULT hr = directInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL);

	assert(hr != DIERR_INVALIDPARAM);
	assert(hr != DIERR_NOINTERFACE);
	assert(hr != DIERR_NOTINITIALIZED);
	if(DIERR_DEVICENOTREG == hr)
		throw runtime_error("Could not create mouse device - device not registered!");
	if(DIERR_OUTOFMEMORY == hr)
		throw runtime_error("Could not create mouse device - out of memory!");

	assert(SUCCEEDED(m_pDevice->SetDataFormat(&c_dfDIMouse2)));

	assert(SUCCEEDED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)));
	
	//if this fails, it will be acquired in the keyboard update function
	//this will lead to the mouse state not being initialized, and may lead to problems
	
	hr = m_pDevice->Acquire();
	assert(DIERR_INVALIDPARAM != hr);
	assert(DIERR_NOTINITIALIZED != hr);
	if(SUCCEEDED(hr))
		m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), (LPVOID)&currentState());
	else
		memset(&currentState(),0,sizeof(DIMOUSESTATE2));

	DIDEVCAPS mouseCapabilities; 
	mouseCapabilities.dwSize = sizeof(mouseCapabilities);
	assert(SUCCEEDED(m_pDevice->GetCapabilities(&mouseCapabilities)));

	if(!(mouseCapabilities.dwFlags & DIDC_ATTACHED))
		throw runtime_error("Mouse is not attached!");

	//m_dwNAxes = mouseCapabilities.dwAxes;
	//m_dwNButtons = mouseCapabilities.dwButtons;

	flipBuffers();
	initialized_=true;

}


void Mouse::Update(void)
{
	assert(initialized_);
	HRESULT hr = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), (LPVOID)&currentState());
	
	switch(hr){
		case DI_OK:
			break; //everything is fine
		case DIERR_INPUTLOST:  
		case DIERR_NOTACQUIRED:
			m_pDevice->Acquire(); //get the device back 
			return; //and try next time again
		case E_PENDING: //not ready yet, maybe next frame
			return;
		default:
			assert(hr != DIERR_NOTINITIALIZED);
			assert(hr != DIERR_INVALIDPARAM);
	}

	/* emit events */
	
	if(currentState().lX || currentState().lY)
		EVENT(MouseMovedEvent(currentState().lX,currentState().lY));

	if(currentState().lZ)
		EVENT(MouseWheelEvent(currentState().lY));

	for(int i=0; i<N_MOUSE_BUTTONS; i++)
	{
		if (Up(currentState().rgbButtons, i) && Down(oldState().rgbButtons, i))
			EVENT(MouseButtonReleasedEvent(i));
		else if (Down(currentState().rgbButtons, i) && Up(oldState().rgbButtons, i))
			EVENT(MouseButtonPressedEvent(i));
	}

	flipBuffers();
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
	//old states because buffers have been swapped already
	return Down(oldState().rgbButtons,button);
} 

}