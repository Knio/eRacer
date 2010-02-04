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
#include "Device.h"

#define GamepadDown(state, index)	(state[index] > 0)
#define GamepadUp(state, index)		(state[index] == 0)



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
	BUTTON_RS,
	N_GAMEPAD_BUTTONS
};


typedef enum GamepadAnalog {
	STICK_LEFT,
	STICK_RIGHT,
	N_GAMEPAD_STICKS
};

/**
 * @brief DirectX Gamepad Wrapper Class
 */
class Gamepad : public Device
{
private:
	DIJOYSTATE2 m_padState;
	DIJOYSTATE2 m_oldPadState;

	bool hasStick1Changed() const;
	bool hasStick2Changed() const;

public:
	IDirectInput8* m_lpdi;

	Gamepad() { }
	~Gamepad() { Shutdown(); }

	/**
	 * @brief Initialization
	 * @param hwnd
	 *			Handle to the window to listen to
	 * @param directInput
	 * 			a pointer to the DirectInput object
	 */
	void Init(	HWND hWnd, IDirectInput8* directInput);

	/**
	 * @brief Call Update every frame to poll the device
	 */
	void Update();



	/**
	 * @brief Get the state of the left analog stick
	 * @return A Vector3 holding axis readings, between -1000 and 1000 each.
	 */
	Vector3 getStick1State();

	/**
	 * @brief Get the state of the left analog stick
	 * @return A Vector3 holding axis readings, between -1000 and 1000 each.
	 */
	Vector3 getStick2State();

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
