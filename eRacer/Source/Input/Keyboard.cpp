/**
 * @file Keyboard.cpp
 * @brief Implementation of the Keyboard class
 *
 * @date 09.01.2010
 * @author: Don Ha, Ole Rehmsen
 */

#include "Keyboard.h"

#include "Core/Event.h"

#include <iostream>
using namespace std;

namespace Input {

Keyboard::Keyboard() : Device() {
}

Keyboard::~Keyboard() { 
	Shutdown(); 
}


void Keyboard::Init(HWND hWnd, IDirectInput8* directInput)
{
	Device::Init(hWnd, directInput);

	handleCreateDeviceReturnCode(directInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL));

	assert(SUCCEEDED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)));
	assert(SUCCEEDED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)));

	//if this fails, it will be acquired in the keyboard update function
	HRESULT hr = m_pDevice->Acquire();
	assert(DIERR_INVALIDPARAM != hr);
	assert(DIERR_NOTINITIALIZED != hr);
	if(SUCCEEDED(hr))
		m_pDevice->GetDeviceState(N_KEYS*sizeof(unsigned char), (void*) currentState());
	else
		memset(currentState(),0,N_KEYS);

	flipBuffers();
	initialized_=true;
}

void Keyboard::Update(void)
{
	Device::Update();

	//poll
	HRESULT hr = m_pDevice->GetDeviceState(N_KEYS*sizeof(unsigned char), (void*) currentState());
	
	switch(hr){
		case DI_OK:
			break; //everything is fine
		case DIERR_INPUTLOST:  
		case DIERR_NOTACQUIRED:
			m_pDevice->Acquire(); //get the device back 
			return;
		case E_PENDING: //not ready yet, maybe next frame
			return;
		default:
			assert(hr != DIERR_NOTINITIALIZED);
			assert(hr != DIERR_INVALIDPARAM);
	}

	/* emit events */
	for (unsigned int i=0; i<N_KEYS; i++)
	{
		if (Up(currentState(), i) && Down(oldState(), i))
			EVENT(KeyReleasedEvent(i));
		else if (Down(currentState(), i) && Up(oldState(), i))
			EVENT(KeyPressedEvent(i));
	}

	flipBuffers();
}

bool Keyboard::isKeyDown(int key)
{
	//old states, because buffers have been swapped already
	return Down(oldState(), key);
}

};