#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <d3d9.h>

namespace Graphics {


class Window
{
  public:
	static Window* window;
	HWND hwnd;
	HINSTANCE hinst;
	int width;
	int height;
	
	Window(int width=800, int height=600): hinst(GetModuleHandle(NULL)), width(width), height(height) {}
	~Window();

	void Create(char* title);
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	int Poll();
};
#endif

};
