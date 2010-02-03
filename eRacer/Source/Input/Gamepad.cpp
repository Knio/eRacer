/**
* @file Gamepad.cpp
* @brief Implementation of the Gamepad class
*
* @date 02.02.2010
* @author: Don Ha
*/

//#include "Core/Event.h"
#include "Gamepad.h"

namespace Input{


BOOL CALLBACK enumCallback(const DIDEVICEINSTANCE* instance, VOID* context)
{
	HRESULT hr;
	Gamepad *mypad = (Gamepad* ) context; 

	// Obtain an interface to the enumerated joystick.
	hr = mypad->m_lpdi->CreateDevice(instance->guidInstance, &mypad->m_lpGamepad, NULL);

	if (FAILED(hr)) { 
		return DIENUM_CONTINUE;
	}
	//Take the first gamepad we find
	return DIENUM_STOP;
}

BOOL CALLBACK enumAxisCallback(const DIDEVICEOBJECTINSTANCE* instance, VOID* context)
{
	HWND hDlg = (HWND)context;

	DIPROPRANGE propRange; 
	propRange.diph.dwSize       = sizeof(DIPROPRANGE); 
	propRange.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
	propRange.diph.dwHow        = DIPH_BYID; 
	propRange.diph.dwObj        = instance->dwType;
	propRange.lMin              = -1000; 
	propRange.lMax              = +1000; 

	Gamepad *mypad = (Gamepad* ) context; 

	// Set the range for the axis
	if (FAILED(mypad->m_lpGamepad->SetProperty(DIPROP_RANGE, &propRange.diph))) {
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}

int Gamepad::Init(HWND hWnd, IDirectInput8* directInput)
{
	m_lpdi = directInput;
	m_lpGamepad = NULL;

	HRESULT hr;

	// Look for the first simple joystick we can find.
	if (FAILED(hr = m_lpdi->EnumDevices(DI8DEVCLASS_GAMECTRL, enumCallback, this, DIEDFL_ATTACHEDONLY))) {
		return hr;
	}

	// Make sure we got a joystick
	if (m_lpGamepad == NULL) {
		return E_FAIL;
	}

	DIDEVCAPS capabilities;

	if (FAILED(hr = m_lpGamepad->SetDataFormat(&c_dfDIJoystick2))) {
		return hr;
	}

	if (FAILED(hr = m_lpGamepad->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_BACKGROUND))) {
		return hr;
	}

	capabilities.dwSize = sizeof(DIDEVCAPS);
	if (FAILED(hr = m_lpGamepad->GetCapabilities(&capabilities))) {
		return hr;
	}

	if (FAILED(hr = m_lpGamepad->EnumObjects(enumAxisCallback, this, DIDFT_AXIS))) {
		return hr;
	}

	return 0;
}

int Gamepad::Update(void)
{

	if (NULL == m_lpGamepad) {
		return S_OK;
	}

	HRESULT hr;
	// Poll the device to read the current state
	hr = m_lpGamepad->Poll(); 
	if (FAILED(hr)) {

		hr = m_lpGamepad->Acquire();
		while (hr == DIERR_INPUTLOST) {
			hr = m_lpGamepad->Acquire();
		}

		if ((hr == DIERR_INVALIDPARAM) || (hr == DIERR_NOTINITIALIZED)) {
			return E_FAIL;
		}

		if (hr == DIERR_OTHERAPPHASPRIO) {
			return S_OK;
		}
	}

	m_oldPadState = m_padState;
	// Get the input's device state
	if (FAILED(hr = m_lpGamepad->GetDeviceState(sizeof(DIJOYSTATE2), &m_padState))) {
		return hr;
	}

	return S_OK;
}

bool Gamepad::isButtonPressed(const GamepadButton &button)
{
	return (m_padState.rgbButtons[button] > 0);
}

Vector3 Gamepad::getControlStickState(const GamepadAnalog &stick)
{
	Vector3 joyState;
	
	if (STICK_LEFT == stick) {
		joyState.x = (float) m_padState.lX;
		joyState.y = (float) m_padState.lY;
		joyState.z = (float) m_padState.lZ;
	}
	else if (STICK_RIGHT == stick) {
		joyState.x = (float) m_padState.lRx;
		joyState.y = (float) m_padState.lRy;
		joyState.z = (float) m_padState.lRz;
	}

	return joyState;
}


void Gamepad::Shutdown(void)
{
	if (NULL != m_lpGamepad) {
		m_lpGamepad->Unacquire();
		m_lpGamepad = NULL;
	}
	if (NULL != m_lpdi) {
		m_lpdi->Release();
		m_lpdi = NULL;
	}
}

};