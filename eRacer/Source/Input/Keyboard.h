#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KeyDown(data, n)	((data[n] & 0x80) ? true : false)
#define KeyUp(data, n)		((data[n] & 0x80) ? false : true)

class Keyboard
{
public:

	//Variables
	LPDIRECTINPUT lpdi;
	LPDIRECTINPUTDEVICE m_keyboard;

	//Function Prototypes
	void init(	HWND hWnd, HINSTANCE hInstance);
	void process();
	void destroy();
	bool isKeyPressed(int key);

	//Constructor + Destructor
	Keyboard() { }
	~Keyboard() { destroy(); }

private:
	unsigned char keystate[256];

};

#endif