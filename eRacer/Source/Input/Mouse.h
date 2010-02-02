/**
 * @file Mouse.h
 * @brief Definition of the Mouse class
 *
 * @date 01.02.2010
 * @author: Ole Rehmsen
 */

#pragma once

#include "Input.h"

namespace Input {

const unsigned int N_MOUSE_BUTTONS = 8;

enum MouseButtons {
	LEFT=0,
	RIGHT=1,
	MIDDLE=2,
	BUTTON4=3,
	BUTTON5=4,
	BUTTON6=5,
	BUTTON7=6,
	BUTTON8=7
};

/**
 * @brief Mouse wrapper class
 */
class Mouse
{
public:
	/**
	 * @brief Constructor stub.
	 */
	Mouse();

	/**
	 * @brief Destructor - release devices and DirectInput
	 */
	~Mouse();

	/**
	 * @brief initialize a mouse
	 *
	 * @param hWnd 
	 *			a handle to the window
	 * @param directInput 
	 *			a pointer to the DirectInput interface
	 * @throws runtime_error 
	 *			if the mouse is not registered, not attached or we are out of memory
	 */
	void Init(HWND hWnd, IDirectInput8* directInput);

	/**
	 * @brief poll the state of of the mouse and emit events
	 */
	void Update();

	/**
	 * @brief Clean up
	 */
	void Shutdown();

	/**
	 * @brief check directly whether a certain mouse button is pressed
	 *
	 * @param mouseButton
	 *			the mouse button to check for (0-7)
	 * @return true if the key is down, false otherwise
	 */
	bool isButtonDown(int mouseButton);
private:
	bool initialized_;

	IDirectInputDevice8* m_pDevice;

	DIMOUSESTATE2       m_States[2];
	bool				m_BufferFlip;

	//for the future
	/*
    bool                 m_bInvertedY;          
    float                m_fSensitivity;        
    float                m_fCursorX;            
    float                m_fCursorY;
    int                  m_iHotSpotX;           
    int                  m_iHotSpotY;
    DWORD                m_dwNAxes;              
    DWORD                m_dwNButtons;           
    DWORD                m_dwCursorColor;       
	DWORD                m_dwScreenWidth; 
    DWORD                m_dwScreenHeight;
	*/

	DIMOUSESTATE2& currentState() { return m_States[m_BufferFlip]; }
	DIMOUSESTATE2& oldState() { return m_States[!m_BufferFlip]; }

	void flipBuffers(){ m_BufferFlip = !m_BufferFlip; }
};






};