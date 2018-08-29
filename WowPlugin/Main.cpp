#include "WowPlugin.h"

HWND				g_hGameWnd = NULL;
int					g_iWidth = 400;
int					g_iHeight = 300;
WowPlugin			g_kWowPlugin;
ULONG_PTR			g_uiToken = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}
		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}
	}
}

BOOL CreateWnd(HINSTANCE hInstance)
{
	WCHAR acWindowClass[256] = L"WowPlugin";

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = acWindowClass;
	wcex.hIconSm = NULL;

	RegisterClassEx(&wcex);

	DEVMODE devmode;
	memset(&devmode, 0, sizeof(devmode));
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);
	int iMaxGraphWidth = devmode.dmPelsWidth;
	int iMaxGraphHeight = devmode.dmPelsHeight;
	int iWindowX = iMaxGraphWidth / 2 - g_iWidth / 2;
	int iWindowY = iMaxGraphHeight / 2 - g_iHeight / 2 - 32;

	g_hGameWnd = CreateWindow(acWindowClass, acWindowClass, (WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN) & ~WS_THICKFRAME, iWindowX, iWindowY, g_iWidth, g_iHeight, NULL, NULL, hInstance, NULL);
	if (!g_hGameWnd)
	{
		return FALSE;
	}

	ShowWindow(g_hGameWnd, SW_SHOW);
	UpdateWindow(g_hGameWnd);
	BringWindowToTop(g_hGameWnd);
	return TRUE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	if (!CreateWnd(hInstance))
	{
		return 0;
	}

	if (!g_kWowPlugin.InitGame())
	{
		return 0;
	}

	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			if (!TranslateAccelerator(msg.hwnd, NULL, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			continue;
		}

		g_kWowPlugin.GameLoop();

		static int s_iFrameTime = timeGetTime();

		int iDeltaTime = timeGetTime() - s_iFrameTime;
		iDeltaTime = 33 - iDeltaTime;

		if (iDeltaTime > 0)
		{
			Sleep(iDeltaTime);
		}

		s_iFrameTime = timeGetTime();
	}

	GdiplusShutdown(g_uiToken);
	return true;
}
