#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers.
#endif

#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include "d3dx12.h"

HWND hwnd = NULL;
LPCTSTR WindowName = L"DX12";
LPCTSTR WindowTitle = L"DX12 Window";

int Width = 800;
int Height = 600;
bool FullScreen = false;

bool InitializeWindow(HINSTANCE hInstace, int ShowWnd, int width, int height, bool fullScreen);
void mainLoop();

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);