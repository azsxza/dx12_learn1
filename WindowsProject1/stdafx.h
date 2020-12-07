#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers.
#endif

#include <windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include "d3dx12.h"
#include <string>

// this is the structure of our constant buffer.
struct ConstantBuffer {
    DirectX::XMFLOAT4 colorMultiplier;
};
#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }

HWND hwnd = NULL;
LPCTSTR WindowName = L"DX12";
LPCTSTR WindowTitle = L"DX12 Window";

int Width = 800;
int Height = 600;
bool FullScreen = false;
bool Running = true;

bool InitializeWindow(HINSTANCE hInstace, int ShowWnd, bool fullScreen);
void mainloop();

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

ID3D12PipelineState* pipelineStateObject;
ID3D12RootSignature* rootSignature;
D3D12_VIEWPORT viewport;
D3D12_RECT scissorRect;
ID3D12Resource* vertexBuffer;
D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

ID3D12Resource* indexBuffer;
D3D12_INDEX_BUFFER_VIEW indexBufferView;

ID3D12Resource* depthStencilBuffer;
ID3D12DescriptorHeap* dsDescriptorHeap;

ID3D12DescriptorHeap* mainDescriptorHeap[frameBufferCount];
ID3D12Resource* constantBufferUploadHeap[frameBufferCount];

ConstantBuffer cbColorMultiplierData;

UINT8* cbColorMultiplierGPUAddress[frameBufferCount];