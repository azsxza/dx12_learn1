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

#define SAFE_RELEASE(p) {if((p)){(p)->Release();(p)=0;}}

HWND hwnd = NULL;
LPCTSTR WindowName = L"DX12";
LPCTSTR WindowTitle = L"DX12 Window";

int Width = 800;
int Height = 600;
bool FullScreen = false;
bool Running = false;

bool InitializeWindow(HINSTANCE hInstace, int ShowWnd, int width, int height, bool fullScreen);
void mainLoop();

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//d3d stuff
const int frameBufferCount = 3;
ID3D12Device* device;
IDXGISwapChain3* swapChain;
ID3D12CommandQueue* commandQueue;
ID3D12DescriptorHeap* rtvDescriptorHeap;
ID3D12Resource* renderTargets[frameBufferCount];
ID3D12CommandAllocator* commandAllocator[frameBufferCount];
ID3D12GraphicsCommandList* commandList;
ID3D12Fence* fence[frameBufferCount];

HANDLE fenceEvent;
UINT64 fenceValue[frameBufferCount];

int frameIndex;
int rtvDescriptorSize;

bool InitD3D();
void Update();
void UpdatePipeline();
void Render();
void Cleanup();
void WaitForPreviousFrame();

