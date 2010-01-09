#include "Keyboard.h"

void Keyboard::init(HWND hWnd, HINSTANCE hInstance)
{
  if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, 
    IID_IDirectInput8, (void**)&lpdi, NULL)))
  {
    // error code
  }
  
  if (FAILED(lpdi->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL)))
    { /* error code */ }
  
  if (FAILED(m_keyboard->SetDataFormat(&c_dfDIKeyboard)))
    { /* error code */ }
  
  if (FAILED(m_keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND |
    DISCL_NONEXCLUSIVE)))
    { /* error code */ }
  
  if (FAILED(m_keyboard->Acquire()))
    { /* error code */ }
}

void Keyboard::process(void)
{
	HRESULT hr = m_keyboard->Acquire();
   while( hr == DIERR_INPUTLOST )
		m_keyboard->Acquire();

  if (FAILED(m_keyboard->GetDeviceState(sizeof(unsigned char[256]), (LPVOID)keystate)))
    { /* error code */ }
}

void Keyboard::destroy(void)
{
	if (m_keyboard) {
		m_keyboard->Unacquire();
	}

	if (lpdi)
		lpdi->Release(); //Should be moved into a global input singleton that can create input objects
}


bool Keyboard::isKeyPressed(int key)
{
  if (KeyDown(keystate, key))
    return true;

  return false;
}