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

	const unsigned int Gamepad::DEAD_RADIUS = 150;


Gamepad::Gamepad(int padNum) 
: Device()
{
	m_padNum = padNum;
	m_lastPadFound = 0;
}

Gamepad::~Gamepad() { 
	Shutdown(); 
}


BOOL CALLBACK enumCallback(const DIDEVICEINSTANCE* instance, VOID* context)
{
	HRESULT hr;
	Gamepad *mypad = (Gamepad* ) context; 

	// Obtain an interface to the enumerated gamepad
	hr = mypad->m_lpdi->CreateDevice(instance->guidInstance, &mypad->m_pDevice, NULL);

	if (FAILED(hr)) { 
		mypad->m_pDevice = NULL;
		return DIENUM_CONTINUE;
	}
	else if (mypad->m_lastPadFound < mypad->m_padNum) { //Take pads in order
		mypad->m_pDevice = NULL;
		mypad->m_lastPadFound++;
		return DIENUM_CONTINUE;
	}

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

	memset(&m_States,0,2*sizeof(DIJOYSTATE2));
	flipBuffers();
	initialized_=true;
}

void Gamepad::Update(void)
{
	Device::Update();


	HRESULT hr;
	// Poll the device to read the current state - we do not have to error check, 
	// because any errors will also occur in the GetDeviceState
	m_pDevice->Poll(); 

	// Get the input's device state
	hr = m_pDevice->GetDeviceState(sizeof(DIJOYSTATE2), &currentState());

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
		if (GamepadUp(currentState().rgbButtons, i) && GamepadDown(oldState().rgbButtons, i)){
			
			EVENT(GamepadButtonReleasedEvent(m_padNum,i));
		}else if (GamepadDown(currentState().rgbButtons, i) && GamepadUp(oldState().rgbButtons, i))
			EVENT(GamepadButtonPressedEvent(m_padNum,i));
	}
	

	//deadzone
	if(abs(currentState().lX) <= DEAD_RADIUS)
		currentState().lX = 0;
	if(abs(currentState().lY) <= DEAD_RADIUS)
		currentState().lY = 0;
	if(abs(currentState().lRx) <= DEAD_RADIUS)
		currentState().lRx = 0;
	if(abs(currentState().lRy) <= DEAD_RADIUS)
		currentState().lRy = 0;
	
	if(hasStick1Changed()){
		EVENT(GamepadStick1AbsoluteEvent(m_padNum,currentState().lX,currentState().lY));
		EVENT(GamepadStick1RelativeEvent(m_padNum,oldState().lX-currentState().lX,
											oldState().lY-currentState().lY));
	}

	if(hasStick2Changed()){
		EVENT(GamepadStick2AbsoluteEvent(m_padNum,currentState().lRx, currentState().lRy));
		EVENT(GamepadStick2RelativeEvent(m_padNum,oldState().lRx-currentState().lRx,
										 oldState().lRy-currentState().lRy));
	}

	if(hasTriggerChanged()){
		EVENT(GamepadTriggerAbsoluteEvent(m_padNum,currentState().lZ));
		EVENT(GamepadTriggerRelativeEvent(m_padNum,oldState().lZ-currentState().lZ));	
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
		|| currentState().lY != oldState().lY;
}


bool Gamepad::hasStick2Changed() const{
	return currentState().lRx != oldState().lRx 
		|| currentState().lRy != oldState().lRy;
}

bool Gamepad::hasTriggerChanged() const{
	return currentState().lZ != oldState().lZ;
}



};