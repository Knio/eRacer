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

void Keyboard::Init(HWND hWnd, IDirectInput* directInput)
{
	//for now, make sure this can only be called once
	assert(NULL == m_pDevice);

	m_BufferFlip = false;
	
	assert(SUCCEEDED(directInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)));
	assert(SUCCEEDED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)));
	assert(SUCCEEDED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)));

	//if this fails, it will be acquired in the keyboard update function
	//this will lead to the key state not being initialized, and may lead to problems
	if(SUCCEEDED(m_pDevice->Acquire()))
		m_pDevice->GetDeviceState(N_KEYS*sizeof(unsigned char), (void*) currentState());
	else
		memset(currentState(),0,N_KEYS);
	

	flipBuffers();
}

HRESULT Keyboard::Update(void)
{

	//poll
	HRESULT hr = m_pDevice->GetDeviceState(N_KEYS*sizeof(unsigned char), (void*) currentState());
	
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
	for (unsigned int i=0; i<N_KEYS; i++)
	{
		if (Up(currentState(), i) && Down(oldState(), i))
			EVENT(KeyReleasedEvent(i));
		else if (Down(currentState(), i) && Up(oldState(), i))
			EVENT(KeyPressedEvent(i));
	}

	flipBuffers();

	return DI_OK; 
}

void Keyboard::Shutdown(void)
{
	if (NULL != m_pDevice) {
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}
}


bool Keyboard::isKeyDown(int key)
{
	//old states, because buffers have been swapped already
	return Down(oldState(), key);
}

};