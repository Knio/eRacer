/**
 * @file Device.cpp
 * @brief Implementation of the Device class
 *
 * @date 03.02.2010
 * @author: Ole Rehmsen
 */

#include "Device.h"

namespace Input {

Device::Device()	
: m_pDevice(NULL),
  m_BufferFlip(false),
  initialized_(false){ 

}

Device::~Device() { 
}



void Device::Init(HWND hWnd, IDirectInput8* directInput)
{
	//for now, make sure this can  only be called once
	assert(!initialized_);
}


void Device::Update(void)
{
	assert(initialized_);
}

void Device::Shutdown(void)
{
	if(NULL != m_pDevice)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}
}

void Device::handleCreateDeviceReturnCode(HRESULT returnCode){
	assert(returnCode != DIERR_INVALIDPARAM);
	assert(returnCode != DIERR_NOINTERFACE);
	assert(returnCode != DIERR_NOTINITIALIZED);
	if(DIERR_DEVICENOTREG == returnCode)
		throw runtime_error("Could not create device - device not registered!");
	if(DIERR_OUTOFMEMORY == returnCode)
		throw runtime_error("Could not create device - out of memory!");
}


};