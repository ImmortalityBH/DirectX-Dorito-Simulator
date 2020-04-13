#include "Graphics.h"

#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

using namespace DirectX;

Graphics::Graphics(int width, int height, HWND hWnd)
{
	HRESULT hr;
	//Describe our Buffer
	DXGI_MODE_DESC bd = {};

	bd.Width = width;
	bd.Height = height;
	bd.RefreshRate.Numerator = 60;
	bd.RefreshRate.Denominator = 1;
	bd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//Describe our SwapChain
	DXGI_SWAP_CHAIN_DESC sd = {};

	sd.BufferDesc = bd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//Create our SwapChain
	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		NULL, nullptr, NULL, D3D11_SDK_VERSION, &sd, &pSwap,
		&pDevice, nullptr, &pContext);

	ID3D11Texture2D* BackBuffer; //Create our BackBuffer
	hr = pSwap->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBuffer));

	//Create our Render Target
	hr = pDevice->CreateRenderTargetView(BackBuffer, nullptr, &pTarget);
	BackBuffer->Release();

	//Set our Render Target
	pContext->OMSetRenderTargets(1, &pTarget, nullptr);

	D3D11_VIEWPORT viewport = {};

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;

	pContext->RSSetViewports(1u, &viewport);
}

Graphics::~Graphics()
{
	if (pSwap != nullptr)
	{
		pSwap->Release();
	}
	if (pDevice != nullptr)
	{
		pDevice->Release();
	}
	if (pContext != nullptr)
	{
		pContext->Release();
	}
	if (pTarget != nullptr)
	{
		pTarget->Release();
	}
}

ID3D11VertexShader* Graphics::createVertexShader(LPCWSTR fileName)
{
	HRESULT hr;
	ID3DBlob* Blob;
	ID3D11VertexShader* pVertexShader;
	hr = D3DReadFileToBlob(fileName, &Blob);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"Failed to read file to blob",
			L"Error", MB_OK | MB_ICONERROR);
	}
	hr = pDevice->CreateVertexShader(Blob->GetBufferPointer(),
		Blob->GetBufferSize(), nullptr, &pVertexShader);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"Failed to create vertex shader",
			L"Error", MB_OK | MB_ICONERROR);
	}
	return pVertexShader;
}

ID3D11PixelShader* Graphics::createPixelShader(LPCWSTR fileName)
{
	HRESULT hr;
	ID3DBlob* Blob;
	ID3D11PixelShader* pPixelShader;
	hr = D3DReadFileToBlob(fileName, &Blob);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"Failed to read file to blob", 
			L"Error", MB_OK | MB_ICONERROR);
	}
	hr = pDevice->CreatePixelShader(Blob->GetBufferPointer(),
		Blob->GetBufferSize(), nullptr, &pPixelShader);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"Failed create pixel shader",
			L"Error", MB_OK | MB_ICONERROR);
	}
	return pPixelShader;
}

void Graphics::Begin(float r, float g, float b)
{
	float color[4] = { r, g, b, 1.0f };
	pContext->ClearRenderTargetView(pTarget, color);

}

void Graphics::End()
{
	pSwap->Present(1u, 0u);
}
