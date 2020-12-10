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
#include <wincodec.h>
#include <fstream>

using namespace DirectX;

// this is the structure of our constant buffer.
struct ConstantBuffer
{
	XMFLOAT4 colorMultiplier;
};

struct ConstantBufferPerObject
{
	XMFLOAT4X4 wvpMat;
};

struct Vertex
{
	Vertex(float x, float y, float z, float u, float v) :pos(x, y, z), texCoord(u,v) {}
	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
};

struct TextVertex
{
	TextVertex(float r, float g, float b, float a, float u, float v, float tw, float th, float x, float y, float w, float h) :
		color(r, g, b, a), texCoord(u, v, tw, th), pos(x, y, w, h) {}
	XMFLOAT4 pos;
	XMFLOAT4 texCoord;
	XMFLOAT4 color;
};

struct FontChar
{
	int id;
	float u, v, twidth, theight;
	float width, height;
	float xoffset, yoffset;
	float xadvance;
};

struct FontKerning
{
	int firstid;
	int secondid;
	float amount;
};

struct Font
{
	std::wstring name;
	std::wstring fontImage;
	int size;
	float lineHeight;
	float baseHeight;
	int textureWidth;
	int textureHeight;
	int numCharacters;
	FontChar* CharList;
	int numKernings;
	FontKerning* KerningsList;
	ID3D12Resource* textureBuffer;
	D3D12_GPU_DESCRIPTOR_HANDLE srvHandle;

	float leftpadding;
	float toppadding;
	float rightpadding;
	float bottompadding;
	float  GetKerning(wchar_t first, wchar_t second)
	{
		for (int i = 0; i < numKernings; i++)
		{
			if ((wchar_t)KerningsList[i].firstid == first && (wchar_t)KerningsList[i].secondid == second)
				return KerningsList[i].amount;
		}
		return 0.0f;
	}

	FontChar* GetChar(wchar_t c)
	{
		for (int i = 0; i < numCharacters; i++)
		{
			if (c == (wchar_t)CharList[i].id)
				return &CharList[i];
		}
		return nullptr;
	}
};

struct Timer
{
	double timerFrequency = 0.0;
	long long lastFrameTime = 0;
	long long lastSecond = 0;
	double frameDelta = 0;
	int fps = 0;

	Timer()
	{
		LARGE_INTEGER li;
		QueryPerformanceFrequency(&li);
		timerFrequency = double(li.QuadPart) / 1000.0;
		QueryPerformanceCounter(&li);
		lastFrameTime = li.QuadPart;
	}

	double GetFrameDelta()
	{
		LARGE_INTEGER li;
		QueryPerformanceFrequency(&li);
		frameDelta = double(li.QuadPart - lastFrameTime) / timerFrequency;
		if (frameDelta > 0)
		{
			fps = 1000 / frameDelta;
		}
		lastFrameTime = li.QuadPart;
		return frameDelta;
	}
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
void Update(double delta);
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


ConstantBuffer cbColorMultiplierData;

UINT8* cbColorMultiplierGPUAddress[frameBufferCount];


int ConstantBufferPerObjectAlignedSize = (sizeof(ConstantBufferPerObject) + 255) & ~255;
ConstantBufferPerObject cbPerObject;
ID3D12Resource* constantBufferUploadHeaps[frameBufferCount];
UINT8* cbvGPUAddress[frameBufferCount];

XMFLOAT4X4 cameraProjMat;
XMFLOAT4X4 cameraViewMat;

XMFLOAT4 cameraPosition;
XMFLOAT4 cameraTarget;
XMFLOAT4 cameraUp;

XMFLOAT4X4 cube1WorldMat;
XMFLOAT4X4 cube1RotMat;
XMFLOAT4 cube1Position;

XMFLOAT4X4 cube2WorldMat;
XMFLOAT4X4 cube2RotMat;
XMFLOAT4 cube2PositionOffset;

int numCubeIndices;

ID3D12Resource* textureBuffer;
int LoadImageDataFromFile(BYTE** imageData, D3D12_RESOURCE_DESC& resourceDescription, LPCWSTR filename, int& bytesPerRow);

DXGI_FORMAT GetDXGIFormatWICFormat(WICPixelFormatGUID& wicFormatGUID);
WICPixelFormatGUID GetCovertToWICFormat(WICPixelFormatGUID& wicFormatGUID);
int GetDXGIFormatBitsPerPixel(DXGI_FORMAT& dxgiFormat);

ID3D12DescriptorHeap* mainDescriptorHeap;
ID3D12Resource* textureBufferUploadHeap;
UINT srvHandleSize;

ID3D12PipelineState* textPSO;
Font arialFont;
int maxNumTextCharacters = 1024;
ID3D12Resource* textVertexBuffer[frameBufferCount];
D3D12_VERTEX_BUFFER_VIEW textVertexBufferView[frameBufferCount];
UINT8* textVBGPUAdderss[frameBufferCount];

Timer timer;
Font LoadFont(LPCWSTR filename, int windowWidth, int windowHeight);
void RenderText(Font font, std::wstring text, XMFLOAT2 pos, XMFLOAT2 scale = XMFLOAT2(1.0f, 1.0f),
	XMFLOAT2 padding = XMFLOAT2(0.5f, 0.5f), XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
