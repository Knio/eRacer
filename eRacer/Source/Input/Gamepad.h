/**
 * @file Gamepad.h
 * @brief This class provides Gamepad Input through DirectX 8
 *
 * @date 09.01.2010
 * @author: Don Ha
 */

#ifndef GAMEPAD_H
#define GAMEPAD_H

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "..\Core\Math.h"


namespace Input{

typedef enum GamepadButton {
	BUTTON_A,
	BUTTON_B,
	BUTTON_X,
	BUTTON_Y,
	BUTTON_LB,
	BUTTON_RB,
	BUTTON_BACK,
	BUTTON_START,
	BUTTON_LS,
	BUTTON_RS
};

typedef enum GamepadAnalog {
	STICK_LEFT,
	STICK_RIGHT
};

/**
 * @brief DirectX Gamepad Wrapper Class
 */
class Gamepad
{
private:
	DIJOYSTATE2 m_padState;
	DIJOYSTATE2 m_oldPadState;

public:
	LPDIRECTINPUTDEVICE8 m_lpGamepad;
	IDirectInput8* m_lpdi;

	Gamepad() { }
	~Gamepad() { Shutdown(); }

/**
 * @brief Initialization
 * @param hwnd
 *			Handle to the window to listen to
 * @param directInput
 * 			a pointer to the DirectInput object
 * @return An error code. When debugging you can check it to see what went wrong.
 */
	int Init(	HWND hWnd, IDirectInput8* directInput);

/**
 * @brief Call Update every frame to poll the device
 * @return Ignore the return value
 */
	int Update();

/**
 * @brief Call Shutdown when done to destroy the device
 */
	void Shutdown();


/**
 * @brief Get the controlstick state using GamepadAnalog sticks
 * @param stick
 *			Choose left or right
 * @return A Vector holding axis readings, between -1000 and 1000.
 */
	Vector3 getControlStickState(const GamepadAnalog &stick);

/**
 * @brief Get the Button state using GamepadButtons sticks
 * @param button
 *			Choose a button
 * @return True if the button is currently pressed
 */
	bool isButtonPressed(const GamepadButton &button);
};


};
#endif
