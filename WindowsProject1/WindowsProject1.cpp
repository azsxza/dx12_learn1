#include "stdafx.h"
#include <iostream>
using namespace std;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	if (!InitializeWindow(hInstance, nShowCmd, Width, Height, FullScreen))
	{
		MessageBox(0, L"Windows Initalization Failed", L"Error", MB_OK);
		return 0;
	}
	mainLoop();

	return 0;
}

bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool fullScreen)
{
	if (fullScreen)
	{
		HMONITOR hmon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(hmon, &mi);

		width = mi.rcMonitor.right;
		height = mi.rcMonitor.top;
	}
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WindowName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Error registering class", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	hwnd = CreateWindowEx(NULL, WindowName, WindowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		width, height, NULL, NULL, hInstance, NULL);
	if (!hwnd)
	{
		auto t = GetLastError();
		MessageBox(NULL, LPCWSTR(GetLastError()), L"Error", MB_OK | MB_ICONERROR);
		MessageBox(NULL, L"Error creating window", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}
	if (fullScreen)
	{
		SetWindowLong(hwnd, GWL_STYLE, 0);
	}
	ShowWindow(hwnd, ShowWnd);
	UpdateWindow(hwnd);

	return true;
}

void mainLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{

		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			if (MessageBox(0, L"Are you sure you want to exit?", L"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)
				DestroyWindow(hWnd);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
