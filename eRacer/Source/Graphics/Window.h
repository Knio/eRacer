#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <d3d9.h>

class Window
{
  public:
	static Window* window;
	HWND hwnd;
	int width;
	int height;
	
	Window(int width=800, int height=600): width(width), height(height) {}
	~Window();

	void Create(char* title);
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	int Poll();
};
#endif
