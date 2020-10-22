#include <cstdio>

#include "DXFW.h"
#include "SystemDefs.h"


LRESULT CALLBACK WndProc (HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

DXFW::DXFW ()
{

}

DXFW::~DXFW ()
{
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings (nullptr, 0);
	}
	UnregisterClass (m_applicationName, m_hInstance);
	m_hInstance = nullptr;
}

bool DXFW::Initialize () 
{
	if (!CreateDXWindow ("DirectX11 - force", WINDOW_POSX, WINDOW_POSY, SCREEN_WIDTH, SCREEN_HEIGHT))
		return false;

	return true;
}

void DXFW::Run ()
{
	MSG msg = {0};
	ZeroMemory (&msg, sizeof (msg));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage (&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}
		else
		{
			// update & render
		}
	}
}

bool DXFW::CreateDXWindow (const char *Title, int x, int y, int width, int height)
{
	HWND hWnd = nullptr;
	WNDCLASSEX wc = {0};

	m_applicationName = const_cast <char *> (Title);
	m_hInstance = GetModuleHandle (nullptr);

	wc.style		 = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc	 = WndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = m_hInstance;
	wc.hIcon		 = LoadIcon (nullptr, IDI_WINLOGO);
	wc.hIconSm		 = wc.hIcon;
	wc.hCursor		 = LoadCursor (nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
	wc.lpszMenuName  = nullptr;
	wc.lpszClassName = m_applicationName;
	wc.cbSize		 = sizeof (WNDCLASSEX);

	if (!RegisterClassEx (&wc))
	{
		MessageBox (nullptr, "RegisterClassEx () failed", "Error", MB_OK);
		return false;
	}

	int screenWidth	 = GetSystemMetrics (SM_CXSCREEN);
	int screenHeight = GetSystemMetrics (SM_CYSCREEN);

	if (FULL_SCREEN)
	{
		DEVMODE dmScreenSettings = {0};
		ZeroMemory (&dmScreenSettings, sizeof (dmScreenSettings));
		dmScreenSettings.dmSize			= sizeof (dmScreenSettings);
		dmScreenSettings.dmPelsWidth	= (unsigned long) screenWidth;
		dmScreenSettings.dmPelsHeight	= (unsigned long) screenHeight;
		dmScreenSettings.dmBitsPerPel	= 32;
		dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings (&dmScreenSettings, CDS_FULLSCREEN);
	}
	else
	{
		screenWidth = width;
		screenHeight = height;
	}

	int nStyle = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX;	
	hWnd = CreateWindowEx (WS_EX_APPWINDOW, Title, Title, nStyle, 
							x, y, screenWidth, screenHeight, nullptr, nullptr, m_hInstance, nullptr);

	if (hWnd == nullptr)
	{
		MessageBox (nullptr, "CreateWindowEx () failed", "Error", MB_OK);
		PostQuitMessage (0);

		return false;
	}

	ShowWindow (hWnd, SW_SHOW);
	SetForegroundWindow (hWnd);
	SetFocus (hWnd);

	return true;
}

LRESULT CALLBACK WndProc (HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps = {0};
	HDC hdc = {0};

	switch (Msg)
	{
		case WM_PAINT:
			{
				hdc = BeginPaint (hWnd, &ps);
				EndPaint (hWnd, &ps);
			} break;
		case WM_CLOSE:
			{
				PostQuitMessage (0);
				DestroyWindow (hWnd);
			} break;
		default:
			{
				return DefWindowProc (hWnd, Msg, wParam, lParam);
			}
	}
	
	return 0;
}