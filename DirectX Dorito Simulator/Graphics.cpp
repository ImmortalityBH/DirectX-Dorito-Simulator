#include "Graphics.h"

#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>
#include "Error.h"
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

	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
	// If the project is in a debug build, enable the debug layer.
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		creationFlags, nullptr, NULL, D3D11_SDK_VERSION, &sd, &pSwap,
		&pDevice, nullptr, &pContext);
	DisplayError(hr, L"Create device and swapchain failed");

	ID3D11Texture2D* BackBuffer; //Create our BackBuffer
	hr = pSwap->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBuffer));
	DisplayError(hr, L"GetBuffer failed");
	//Create our Render Target
	hr = pDevice->CreateRenderTargetView(BackBuffer, nullptr, &pTarget);
	DisplayError(hr, L"Create Render Target view failed");
	BackBuffer->Release();

	D3D11_TEXTURE2D_DESC ds = {};

	ds.Width = width;
	ds.Height = height;
	ds.MipLevels = 1;
	ds.ArraySize = 1;
	ds.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	ds.SampleDesc.Count = 1;
	ds.SampleDesc.Quality = 0;
	ds.Usage = D3D11_USAGE_DEFAULT;
	ds.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	ds.CPUAccessFlags = 0;
	ds.MiscFlags = 0;

	hr = pDevice->CreateTexture2D(&ds, nullptr, &pDepthStencilBuffer);
	hr = pDevice->CreateDepthStencilView(pDepthStencilBuffer, nullptr, &pDepthStencilView);

	//Set our Render Target
	pContext->OMSetRenderTargets(1, &pTarget, nullptr);

	//D3D11_RASTERIZER_DESC rsd;
	//rsd.FillMode = D3D11_FILL_WIREFRAME;

	//hr = pDevice->CreateRasterizerState(&rsd, &pWireframeState);
	///DisplayError(hr, L"Create Rasterizer State failed");
	
	D3D11_VIEWPORT viewport = {};

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	pContext->RSSetViewports(1u, &viewport);
}

Graphics::~Graphics()
{
	if (pSwap) pSwap->Release();
	if (pDevice) pDevice->Release();
	if (pContext) pContext->Release();
	if (pTarget) pTarget->Release();
	if (pDepthStencilBuffer) pDepthStencilBuffer->Release();
	if (pDepthStencilView) pDepthStencilView->Release();
	if (pWireframeState) pWireframeState->Release();
}

ID3D11VertexShader* Graphics::createVertexShader(LPCWSTR fileName, ID3DBlob** ppBlob)
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

ID3D11PixelShader* Graphics::createPixelShader(LPCWSTR fileName, ID3DBlob** ppBlob)
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
	pContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Graphics::End()
{
	pSwap->Present(1u, 0u);
}
