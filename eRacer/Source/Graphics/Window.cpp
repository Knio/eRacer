#include "Window.h"
#include "Core/Event.h"

namespace Graphics {

Window *Window::window = NULL;
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return Window::window->MsgProc(hWnd, msg, wParam, lParam);
}

void Window::Create(char* title)
{
	// Register the window class
    WNDCLASSEX wc;
	wc.cbSize = sizeof( WNDCLASSEX );
    wc.style = CS_CLASSDC;
	wc.lpfnWndProc = Graphics::MsgProc;
    wc.cbClsExtra = 0L;
    wc.cbWndExtra = 0L;
    wc.hInstance = GetModuleHandle( NULL );
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
	wc.lpszClassName = "eRacer";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx( &wc );

    // Create the application's window
	window = this;
    hwnd = CreateWindow("eRacer", title,
                        WS_OVERLAPPEDWINDOW, 100, 20, width, height,
                        NULL, NULL, wc.hInstance, NULL );
	ShowWindow( hwnd, SW_SHOWDEFAULT );
    UpdateWindow( hwnd );
}
Window::~Window()
{
	// UnregisterClass( L"eRacer Window", wc.hInstance );
}

int Window::Poll()
{
    MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (msg.message == WM_QUIT)
	{
		EVENT(QuitEvent());
		return 0;
	}
	return 1;
}

LRESULT Window::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;

        case WM_PAINT:
            ValidateRect( hWnd, NULL );
            return 0;
    }
    return DefWindowProc( hWnd, msg, wParam, lParam );
}

};
