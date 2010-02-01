/**
 * @file Keyboard.cpp
 * @brief Implementation of the Keyboard class
 *
 * @date 09.01.2010
 * @author: Don Ha
 */

#include "Core/Event.h"
#include "Input.h"


void Input::Init(HWND hWnd, HINSTANCE hInstance)
{
	assert(SUCCEEDED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_lpdi, NULL)));

	assert(SUCCEEDED(m_lpdi->CreateDevice(GUID_SysKeyboard, &m_lpKeyboard, NULL)));
	assert(SUCCEEDED(m_lpKeyboard->SetDataFormat(&c_dfDIKeyboard)));
	assert(SUCCEEDED(m_lpKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)));
	assert(SUCCEEDED(m_lpKeyboard->Acquire()));
	assert(SUCCEEDED(m_lpKeyboard->GetDeviceState(sizeof(unsigned char[256]), (void*) currentKeyState())));

	DIDEVCAPS mouseCapabilities; 

	assert(SUCCEEDED(m_lpdi->CreateDevice(GUID_SysMouse, &m_lpMouse, NULL)));
	assert(SUCCEEDED(m_lpMouse->SetDataFormat(&c_dfDIMouse2)));
	assert(SUCCEEDED(m_lpMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)));
	assert(SUCCEEDED(m_lpMouse->Acquire()));
	assert(SUCCEEDED(m_lpMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (LPVOID)&currentMouseState())));

	mouseCapabilities.dwSize = sizeof(mouseCapabilities);
	m_lpMouse->GetCapabilities(&mouseCapabilities);

	assert(mouseCapabilities.dwFlags & DIDC_ATTACHED);

	m_dwNAxes = mouseCapabilities.dwAxes;
	m_dwNButtons = mouseCapabilities.dwButtons;

	flipBuffers();

}

int Input::Update(void)
{
	//TODO should we not check instead of always reacquiring?
	//Reacquire the keyboard on loss. Could potentially infinite loop
	HRESULT hr = m_lpKeyboard->Acquire();
	while( hr == DIERR_INPUTLOST )
		m_lpKeyboard->Acquire();

	if (FAILED(m_lpKeyboard->GetDeviceState(N_KEYS*sizeof(unsigned char), (void*) currentKeyState())))
		return -1;


	//Events should be triggered here

	for (int i=0;i<N_KEYS;i++)
	{
		if (!KeyDown(currentKeyState(), i) && KeyDown(oldKeyState(), i))
		{
			// a key was released
			EVENT(KeyReleasedEvent(i));
		}
		if (KeyDown(currentKeyState(), i) && !KeyDown(oldKeyState(), i))
		{
			// a key was pressed
			EVENT(KeyPressedEvent(i));
		}
	}



	if(DIERR_INPUTLOST == m_lpMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (LPVOID)&currentMouseState()))
		m_lpMouse->Acquire();

	for(int i=0; i<N_MOUSE_BUTTONS; i++)
	{
		if (!KeyDown(currentMouseState().rgbButtons, i) && KeyDown(oldMouseState().rgbButtons, i))
		{
			// a key was released
			EVENT(MouseButtonReleasedEvent(i));
		}
		if (KeyDown(currentMouseState().rgbButtons, i) && !KeyDown(oldMouseState().rgbButtons, i))
		{
			// a key was pressed
			EVENT(MouseButtonPressedEvent(i));
		}
	}
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


bool Input::isKeyPressed(int key)
{
	return KeyDown(currentKeyState(), key);
}

bool Input::isMouseButtonDown(int button)
{
	return KeyDown(currentMouseState().rgbButtons,button);
} 
