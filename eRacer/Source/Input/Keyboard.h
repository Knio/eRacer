/**
 * @file Keyboard.h
 * @brief Definition of the Keyboard class
 *
 * @date 09.01.2010
 * @author: Don Ha, Ole Rehmsen
 */

#pragma once

#include "Input.h"

//TODO should be constant
#define N_KEYS				256

namespace Input{

/**
 * @brief Keyboard wrapper class
 */
class Keyboard
{
private:
	LPDIRECTINPUTDEVICE m_pDevice;

	unsigned char		m_States[2*N_KEYS];
	bool				m_BufferFlip;


	unsigned char* currentState() { return m_States+m_BufferFlip*N_KEYS; }
	unsigned char* oldState() { return m_States+(!m_BufferFlip)*N_KEYS; }

	void flipBuffers(){ m_BufferFlip = !m_BufferFlip; }
public:
	/**
	 * @brief initialize a keyboard
	 *
	 * @param hWnd 
	 *			a handle to the window
	 * @param directInput 
	 *			a pointer to the DirectInput Interface
	 */
	void Init(HWND hWnd, IDirectInput* directInput);

	/**
	 * @brief poll the state of the input devices and emit events
	 *
	 * @returns 
	 *		 0 on success or
	 *		-1 if one of the devices was not ready or lost the connection - just poll again!
	 */
	//TODO update doc
	HRESULT Update();

	/**
	 * @brief Clean up
	 */
	void Shutdown();

	/**
	 * @brief check directly whether a certain key is pressed
	 *
	 * @param key
	 *			the key to check for
	 * @return true if the key is down, false otherwise
	 */
	bool isKeyDown(int key);

	/**
	 * @brief Constructor stub.
	 */
	Keyboard() : m_pDevice(NULL) { }

	/**
	 * @brief Destructor - release devices and DirectInput
	 */
	~Keyboard() { Shutdown(); }
};


//-----------------------------------------------------------------------------
// Keyboard Key definitions, for wrapping DirectInput
//-----------------------------------------------------------------------------

//TODO should be constants
#define KEY_RELEASED        FALSE
#define KEY_PRESSED         TRUE

#define KEY_UNLOCKED        FALSE
#define KEY_LOCKED          TRUE

#define KEY_ESC             DIK_ESCAPE
#define KEY_ESCAPE          DIK_ESCAPE

#define KEY_MINUS           DIK_MINUS
#define KEY_EQUALS          DIK_EQUALS

#define KEY_BACKSPACE       DIK_BACK

#define KEY_TAB             DIK_TAB

#define KEY_LBRACKET        DIK_LBRACKET
#define KEY_RBRACKET        DIK_RBRACKET

#define KEY_ENTER           DIK_RETURN
#define KEY_RETURN          DIK_RETURN

#define KEY_CTRL            DIK_LCONTROL
#define KEY_LCTRL           DIK_LCONTROL
#define KEY_RCTRL           DIK_RCTRL

#define KEY_SHIFT           DIK_LSHIFT
#define KEY_LSHIFT          DIK_LSHIFT
#define KEY_RSHIFT          DIK_RSHIFT

#define KEY_ALT             DIK_LMENU
#define KEY_LALT            DIK_LMENU
#define KEY_RALT            DIK_RMENU

#define KEY_0               DIK_0
#define KEY_1               DIK_1
#define KEY_2               DIK_2
#define KEY_3               DIK_3
#define KEY_4               DIK_4
#define KEY_5               DIK_5
#define KEY_6               DIK_6
#define KEY_7               DIK_7
#define KEY_8               DIK_8
#define KEY_9               DIK_9

#define KEY_A               DIK_A
#define KEY_B               DIK_B
#define KEY_C               DIK_C
#define KEY_D               DIK_D
#define KEY_E               DIK_E
#define KEY_F               DIK_F
#define KEY_G               DIK_G
#define KEY_H               DIK_H
#define KEY_I               DIK_I
#define KEY_J               DIK_J
#define KEY_K               DIK_K
#define KEY_L               DIK_L
#define KEY_M               DIK_M
#define KEY_N               DIK_N
#define KEY_O               DIK_O
#define KEY_P               DIK_P
#define KEY_Q               DIK_Q
#define KEY_R               DIK_R
#define KEY_S               DIK_S
#define KEY_T               DIK_T
#define KEY_U               DIK_U
#define KEY_V               DIK_V
#define KEY_W               DIK_W
#define KEY_X               DIK_X
#define KEY_Y               DIK_Y
#define KEY_Z               DIK_Z

#define KEY_SEMICOLON       DIK_SEMICOLON
#define KEY_APOSTROPHE      DIK_APOSTROPHE

#define KEY_TILDE           DIK_GRAVE
#define KEY_GRAVE           DIK_GRAVE

#define KEY_BACKSLASH       DIK_BACKSLASH

#define KEY_COMMA           DIK_COMMA
#define KEY_PERIOD          DIK_PERIOD
#define KEY_FORWARDSLASH    DIK_SLASH
#define KEY_SLASH           DIK_SLASH

#define KEY_SPACE           DIK_SPACE

#define KEY_CAPSLOCK        DIK_CAPITAL
#define KEY_CAPITAL         DIK_CAPITAL

#define KEY_F1              DIK_F1
#define KEY_F2              DIK_F2
#define KEY_F3              DIK_F3
#define KEY_F4              DIK_F4
#define KEY_F5              DIK_F5
#define KEY_F6              DIK_F6
#define KEY_F7              DIK_F7
#define KEY_F8              DIK_F8
#define KEY_F9              DIK_F9
#define KEY_F10             DIK_F10
#define KEY_F11             DIK_F11
#define KEY_F12             DIK_F12

#define KEY_SYSRQ           DIK_SYSRQ
#define KEY_SCROLLLOCK      DIK_SCROLL
#define KEY_PAUSE           DIK_PAUSE

#define KEY_NUMLOCK         DIK_NUMLOCK
#define KEY_NUMPAD0         DIK_NUMPAD0
#define KEY_NUMPAD1         DIK_NUMPAD1
#define KEY_NUMPAD2         DIK_NUMPAD2
#define KEY_NUMPAD3         DIK_NUMPAD3
#define KEY_NUMPAD4         DIK_NUMPAD4
#define KEY_NUMPAD5         DIK_NUMPAD5
#define KEY_NUMPAD6         DIK_NUMPAD6
#define KEY_NUMPAD7         DIK_NUMPAD7
#define KEY_NUMPAD8         DIK_NUMPAD8
#define KEY_NUMPAD9         DIK_NUMPAD9
#define KEY_ADD             DIK_ADD
#define KEY_SUBTRACT        DIK_SUBTRACT
#define KEY_DIVIDE          DIK_DEVICE
#define KEY_MULTIPLY        DIK_MULTIPLY
#define KEY_DECIMAL         DIK_DECIMAL
#define KEY_NUMPADENTER     DIK_NUMPADENTER

#define KEY_INSERT          DIK_INSERT
#define KEY_DELETE          DIK_DELETE
#define KEY_HOME            DIK_HOME
#define KEY_END             DIK_END
#define KEY_PAGEUP          DIK_PRIOR
#define KEY_PAGEDOWN        DIK_NEXT

#define KEY_UP              DIK_UP
#define KEY_DOWN            DIK_DOWN
#define KEY_LEFT            DIK_LEFT
#define KEY_RIGHT           DIK_RIGHT

#define KEY_LWIN            DIK_LWIN
#define KEY_RWIN            DIK_RWIN
#define KEY_APPS            DIK_APPS

#define MOUSE_LEFTBUTTON   0
#define MOUSE_RIGHTBUTTON  1
#define MOUSE_MIDDLEBUTTON 2
#define MOUSE_4BUTTON      3
#define MOUSE_5BUTTON      4
#define MOUSE_6BUTTON      5
#define MOUSE_7BUTTON      6
#define MOUSE_8BUTTON      7

};
