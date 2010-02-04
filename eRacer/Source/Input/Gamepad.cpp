/**
* @file Gamepad.cpp
* @brief Implementation of the Gamepad class
*
* @date 02.02.2010
* @author: Don Ha
*/

#include "Core/Event.h"
#include "Gamepad.h"

namespace Input{


BOOL CALLBACK enumCallback(const DIDEVICEINSTANCE* instance, VOID* context)
{
	HRESULT hr;
	Gamepad *mypad = (Gamepad* ) context; 

	// Obtain an interface to the enumerated gamepad
	hr = mypad->m_lpdi->CreateDevice(instance->guidInstance, &mypad->m_pDevice, NULL);

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
	if (FAILED(mypad->m_pDevice->SetProperty(DIPROP_RANGE, &propRange.diph))) {
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}

void Gamepad::Init(HWND hWnd, IDirectInput8* directInput)
{
	Device::Init(hWnd,directInput);

	m_lpdi = directInput;


	// Look for the first simple joystick we can find.
	assert(SUCCEEDED(m_lpdi->EnumDevices(DI8DEVCLASS_GAMECTRL, enumCallback, this, DIEDFL_ATTACHEDONLY)));


	// Make sure we got a device
	if (m_pDevice == NULL)
		throw runtime_error("Could find any gamepads!");


	assert(SUCCEEDED(m_pDevice->SetDataFormat(&c_dfDIJoystick2)));

	assert(SUCCEEDED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_BACKGROUND)));

	DIDEVCAPS capabilities;
	capabilities.dwSize = sizeof(DIDEVCAPS);
	assert(SUCCEEDED(m_pDevice->GetCapabilities(&capabilities)));

	assert(SUCCEEDED(m_pDevice->EnumObjects(enumAxisCallback, this, DIDFT_AXIS)));

	flipBuffers();
	initialized_=true;

}

void Gamepad::Update(void)
{
	Device::Update();

	//if we do not have a device, just do nothing
	if (NULL == m_pDevice) {
		return;
	}

	HRESULT hr;
	// Poll the device to read the current state - we do not have to error check, 
	// because any errors will also occur in the GetDeviceState
	m_pDevice->Poll(); 

	// Get the input's device state
	hr = m_pDevice->GetDeviceState(sizeof(DIJOYSTATE2), &oldState());

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

	for(unsigned int i=0; i<N_GAMEPAD_BUTTONS; i++)
	{
		if (GamepadUp(currentState().rgbButtons, i) && GamepadDown(oldState().rgbButtons, i))
			EVENT(GamepadButtonReleasedEvent(i));
		else if (GamepadDown(currentState().rgbButtons, i) && GamepadUp(oldState().rgbButtons, i))
			EVENT(GamepadButtonPressedEvent(i));
	}

	if(hasStick1Changed()){
		EVENT(GamepadStick1ChangedAbsoluteEvent(currentState().lX,currentState().lY,currentState().lZ));
		EVENT(GamepadStick1ChangedRelativeEvent(oldState().lX-currentState().lX,
												oldState().lY-currentState().lY,
												oldState().lZ-currentState().lZ));
	}

	if(hasStick2Changed()){
		EVENT(GamepadStick1ChangedAbsoluteEvent(currentState().lRx, currentState().lRy, currentState().lRz));
		EVENT(GamepadStick1ChangedRelativeEvent(oldState().lRx-currentState().lRx,
												oldState().lRy-currentState().lRy,
												oldState().lRz-currentState().lRz));
	}

	flipBuffers();

}

bool Gamepad::isButtonPressed(const GamepadButton &button) const
{
	return GamepadDown(oldState().rgbButtons,button);
}

Vector3 Gamepad::getStick1State() const
{
	Vector3 joyState;
	
	joyState.x = (float) oldState().lX;
	joyState.y = (float) oldState().lY;
	joyState.z = (float) oldState().lZ;

	return joyState;
}

Vector3 Gamepad::getStick2State() const{
	Vector3 joyState;
	
	joyState.x = (float) oldState().lRx;
	joyState.y = (float) oldState().lRy;
	joyState.z = (float) oldState().lRz;

	return joyState;
	
}


bool Gamepad::hasStick1Changed() const{
	return currentState().lX != oldState().lX 
		|| currentState().lY != oldState().lY 
		|| currentState().lZ != oldState().lZ;
}

bool Gamepad::hasStick2Changed() const{
	return currentState().lRx != oldState().lRx 
		|| currentState().lRy != oldState().lRy 
		|| currentState().lRz != oldState().lRz;
}


};