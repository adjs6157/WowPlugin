#include "WowPlugin.h"

HWND				g_hGameWnd = NULL;
int					g_iWidth = 400;
int					g_iHeight = 300;
WowPlugin			g_kWowPlugin;
ULONG_PTR			g_uiToken = 0;
HINSTANCE			g_hInstance = 0;

HWND g_hButtonCurrPut = 0;
HWND g_hButtonAltStopAttack = 0;
HWND g_hButtonAutoFish = 0;

void CreateControl(HWND hWnd)
{
	g_hButtonCurrPut = (HWND)CreateWindow(TEXT("Button"),  //Button是预定义 窗体类
		TEXT("自动治疗宠物"),
		WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
		10, 45, 160, 35,
		hWnd,
		(HMENU)520,  //(重点)这里设置按钮id,但是 原本是设置菜单的 所以需要HMENU
		g_hInstance,
		NULL);

	g_hButtonAltStopAttack = (HWND)CreateWindow(TEXT("Button"),  //Button是预定义 窗体类
		TEXT("ALT键停止攻击"),
		WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
		10, 85, 160, 35,
		hWnd,
		(HMENU)521,  //(重点)这里设置按钮id,但是 原本是设置菜单的 所以需要HMENU
		g_hInstance,
		NULL);

	g_hButtonAutoFish = (HWND)CreateWindow(TEXT("Button"),  //Button是预定义 窗体类
		TEXT("自动钓鱼"),
		WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
		10, 125, 160, 35,
		hWnd,
		(HMENU)522,  //(重点)这里设置按钮id,但是 原本是设置菜单的 所以需要HMENU
		g_hInstance,
		NULL);
}

void ProcessControl(HWND hWnd, DWORD wParam)
{
	if (LOWORD(wParam) == 520 && HIWORD(wParam) == BN_CLICKED)
	{
		if (SendMessage(g_hButtonCurrPut, BM_GETCHECK, 0, 0) == BST_CHECKED)
		{
			SendMessage(g_hButtonCurrPut, BM_SETCHECK, BST_UNCHECKED, 0);
			g_kWowPlugin.SetIsCurePet(false);
		}
		else
		{
			SendMessage(g_hButtonCurrPut, BM_SETCHECK, BST_CHECKED, 0);
			g_kWowPlugin.SetIsCurePet(true);
		}
	}
	else if (LOWORD(wParam) == 521 && HIWORD(wParam) == BN_CLICKED)
	{
		if (SendMessage(g_hButtonAltStopAttack, BM_GETCHECK, 0, 0) == BST_CHECKED)
		{
			SendMessage(g_hButtonAltStopAttack, BM_SETCHECK, BST_UNCHECKED, 0);
			g_kWowPlugin.SetIsAltStopAttack(false);
		}
		else
		{
			SendMessage(g_hButtonAltStopAttack, BM_SETCHECK, BST_CHECKED, 0);
			g_kWowPlugin.SetIsAltStopAttack(true);
		}
	}
	else if (LOWORD(wParam) == 522 && HIWORD(wParam) == BN_CLICKED)
	{
		if (SendMessage(g_hButtonAutoFish, BM_GETCHECK, 0, 0) == BST_CHECKED)
		{
			SendMessage(g_hButtonAutoFish, BM_SETCHECK, BST_UNCHECKED, 0);
			g_kWowPlugin.SetIsAutoFish(false);
		}
		else
		{
			SendMessage(g_hButtonAutoFish, BM_SETCHECK, BST_CHECKED, 0);
			g_kWowPlugin.SetIsAutoFish(true);
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	HDC hdc; //Handle Device Context
	PAINTSTRUCT ps;
	RECT rect;
	switch (message)
	{
		case WM_PAINT:
			//hdc = BeginPaint(hWnd, &ps);
			//GetClientRect(hWnd, &rect);
			//DrawText(hdc, TEXT("叫我涵涵"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			//EndPaint(hWnd, &ps);
			DefWindowProc(hWnd, message, wParam, lParam);
			break;
		case WM_CREATE:
			CreateControl(hWnd);
			DefWindowProc(hWnd, message, wParam, lParam);
			break;
		case WM_COMMAND:
			ProcessControl(hWnd, wParam);
			DefWindowProc(hWnd, message, wParam, lParam);
			break;
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

	g_hInstance = hInstance;

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
