/**
 * @file Input.cpp
 * @brief Implementation of the Input class
 *
 * @date 09.01.2010
 * @author: Don Ha, Ole Rehmsen
 */

#include "Core/Event.h"
#include "Input.h"

#include <iostream>
using namespace std;


void Input::Init(HWND hWnd, HINSTANCE hInstance)
{
	//TODO make safe for reinitialisation
	m_BufferFlip = false;
	assert(SUCCEEDED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_lpdi, NULL)));

	assert(SUCCEEDED(m_lpdi->CreateDevice(GUID_SysKeyboard, &m_lpKeyboard, NULL)));
	assert(SUCCEEDED(m_lpKeyboard->SetDataFormat(&c_dfDIKeyboard)));
	assert(SUCCEEDED(m_lpKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)));

	//if this fails, it will be acquired in the keyboard update function
	//this will lead to the key state not being initialized, and may lead to problems
	if(SUCCEEDED(m_lpKeyboard->Acquire()))
		m_lpKeyboard->GetDeviceState(N_KEYS*sizeof(unsigned char), (void*) currentKeyState());

	DIDEVCAPS mouseCapabilities; 

	assert(SUCCEEDED(m_lpdi->CreateDevice(GUID_SysMouse, &m_lpMouse, NULL)));
	assert(SUCCEEDED(m_lpMouse->SetDataFormat(&c_dfDIMouse2)));
	assert(SUCCEEDED(m_lpMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)));
	
	//if this fails, it will be acquired in the keyboard update function
	//this will lead to the mouse state not being initialized, and may lead to problems
	if(SUCCEEDED(m_lpMouse->Acquire()))
		m_lpMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (LPVOID)&currentMouseState());

	mouseCapabilities.dwSize = sizeof(mouseCapabilities);
	m_lpMouse->GetCapabilities(&mouseCapabilities);

	assert(mouseCapabilities.dwFlags & DIDC_ATTACHED);

	//m_dwNAxes = mouseCapabilities.dwAxes;
	//m_dwNButtons = mouseCapabilities.dwButtons;

	flipBuffers();

}

HRESULT Input::pollKeyboard(){
	//poll
	HRESULT hr = m_lpKeyboard->GetDeviceState(N_KEYS*sizeof(unsigned char), (void*) currentKeyState());
	
	switch(hr){
		case DI_OK:
			break; //everything is fine
		case DIERR_INPUTLOST:  
		case DIERR_NOTACQUIRED:
			m_lpKeyboard->Acquire(); //get the device back 
			return hr;
		case E_PENDING: //not ready yet, maybe next frame
			return hr;
		default:
			assert(hr != DIERR_NOTINITIALIZED);
	}

	/* emit events */
	for (unsigned int i=0; i<N_KEYS; i++)
	{
		if (!KeyDown(currentKeyState(), i) && KeyDown(oldKeyState(), i))
			EVENT(KeyReleasedEvent(i));
		else if (KeyDown(currentKeyState(), i) && !KeyDown(oldKeyState(), i))
			EVENT(KeyPressedEvent(i));
	}

	return DI_OK; 
}
HRESULT Input::pollMouse(){
	HRESULT hr = m_lpMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (LPVOID)&currentMouseState());
	
	switch(hr){
		case DI_OK:
			break; //everything is fine
		case DIERR_INPUTLOST:  
		case DIERR_NOTACQUIRED:
			m_lpMouse->Acquire(); //get the device back 
			return hr;
		case E_PENDING: //not ready yet, maybe next frame
			return hr;
		default:
			assert(hr != DIERR_NOTINITIALIZED);
	}

	/* emit events */
	
	if(currentMouseState().lX || currentMouseState().lY)
		EVENT(MouseMovedEvent(currentMouseState().lX,currentMouseState().lY));

	if(currentMouseState().lZ)
		EVENT(MouseWheelEvent(currentMouseState().lY));

	for(int i=0; i<N_MOUSE_BUTTONS; i++)
	{
		if (!KeyDown(currentMouseState().rgbButtons, i) && KeyDown(oldMouseState().rgbButtons, i))
			EVENT(MouseButtonReleasedEvent(i));
		else if (KeyDown(currentMouseState().rgbButtons, i) && !KeyDown(oldMouseState().rgbButtons, i))
			EVENT(MouseButtonPressedEvent(i));
	}

	return DI_OK; 
}


int Input::Update(void)
{
	if(FAILED(pollKeyboard()) || FAILED(pollMouse()))
		return -1;

	flipBuffers();

	return 0;
}

void Input::Shutdown(void)
{
	if (NULL != m_lpKeyboard) {
		m_lpKeyboard->Unacquire();
		m_lpKeyboard->Release();
		m_lpKeyboard = NULL;
	}
	if(NULL != m_lpMouse)
	{
		m_lpMouse->Unacquire();
		m_lpMouse->Release();
		m_lpMouse = NULL;
	}

	if (NULL != m_lpdi) {
		m_lpdi->Release();
	}

}


bool Input::isKeyDown(int key)
{
	return KeyDown(currentKeyState(), key);
}

bool Input::isMouseButtonDown(int button)
{
	return KeyDown(currentMouseState().rgbButtons,button);
} 
