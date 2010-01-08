//-----------------------------------------------------------------------------
// File: Main.cpp
// Jan 7, 2010 
//
// Built off of DirectX 9 SDK Samples
// Wallsocket Studios
//-----------------------------------------------------------------------------
#include "Graphics\GraphicsLayer.h"

//-----------------------------------------------------------------------------
// Name: ftime()
// Desc: This helper function gets the time since last reboot in seconds
// It has more than microsecond accuracy
// This function needs to be optimized
//-----------------------------------------------------------------------------
double ftime()
{
	LARGE_INTEGER clockSpeed;
	LARGE_INTEGER tick;
	QueryPerformanceFrequency(&clockSpeed);
	QueryPerformanceCounter(&tick);
	double elapsedSeconds = ((double) tick.QuadPart )/ clockSpeed.QuadPart;
	return (elapsedSeconds);
}


//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
			Graphics()->Shutdown();
            PostQuitMessage( 0 );
            return 0;

        case WM_PAINT:
            ValidateRect( hWnd, NULL );
            return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

//-----------------------------------------------------------------------------
// Name: wWinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
	// Register the window class
    WNDCLASSEX wc;
	wc.cbSize = sizeof( WNDCLASSEX );
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = MsgProc;
    wc.cbClsExtra = 0L;
    wc.cbWndExtra = 0L;
    wc.hInstance = GetModuleHandle( NULL );
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
	wc.lpszClassName = L"eRacer Window";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx( &wc );

    // Create the application's window
    HWND hWnd = CreateWindow( L"eRacer Window", L"eRacer Alpha 0.01",
                              WS_OVERLAPPEDWINDOW, 100, 100, 1024, 768,
                              NULL, NULL, wc.hInstance, NULL );

	//Timing Variables
	double timeAccum = 0;
	double frameEnd = 0;
	double frameStart = ftime();
	double frameTime = 0;

    // Initialize Direct3D
	if (Graphics()->Init(hWnd) == S_OK)
    {
        // Show the window
        ShowWindow( hWnd, SW_SHOWDEFAULT );
        UpdateWindow( hWnd );

        // Enter the message pump
        MSG msg;
		ZeroMemory(&msg, sizeof(MSG));
		while(msg.message != WM_QUIT) {
			//Handle the Default Windows Messages
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			} 
			else {
				//Some inefficient time tracking code
				frameEnd = ftime();
				frameTime = frameEnd - frameStart;
				frameStart = frameEnd;
				timeAccum += frameTime;

				//Run the game
				//This game loop will consume 100% available CPU
				//One logic step per rendered frame
											//Input goes here
											//Logic goes here
				Graphics()->RenderFrame();	//Graphics
			}
		}
    }

    UnregisterClass( L"eRacer Window", wc.hInstance );
    return 0;
}
