#include "Graphics.h"

#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>
#include "Error.h"
#include "d3dUtil.h"

using namespace DirectX;

Graphics::~Graphics()
{
	ReleaseCOM(pSwapChain);
	ReleaseCOM(pDevice);
	ReleaseCOM(pContext);
	ReleaseCOM(pTarget);
	ReleaseCOM(pDepthStencilBuffer);
	ReleaseCOM(pDepthStencilView);
	ReleaseCOM(pWireframeState);
}

bool Graphics::init(unsigned int width, unsigned int height, HWND hWnd)
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
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
	// If the project is in a debug build, enable the debug layer.
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		creationFlags, nullptr, NULL, D3D11_SDK_VERSION, &sd, &pSwapChain,
		&pDevice, nullptr, &pContext);
	DisplayError(hr, L"Create device and swapchain failed");

	ID3D11Texture2D* BackBuffer; //Create our BackBuffer
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBuffer));
	DisplayError(hr, L"GetBuffer failed");
	//Create our Render Target
	hr = pDevice->CreateRenderTargetView(BackBuffer, nullptr, &pTarget);
	DisplayError(hr, L"Create Render Target view failed");
	ReleaseCOM(BackBuffer);

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
	pContext->OMSetRenderTargets(1, &pTarget, pDepthStencilView);

	D3D11_RASTERIZER_DESC rsd = {};
	rsd.FillMode = D3D11_FILL_WIREFRAME;
	rsd.CullMode = D3D11_CULL_NONE;

	hr = pDevice->CreateRasterizerState(&rsd, &pWireframeState);
	DisplayError(hr, L"Create Rasterizer State failed");

	D3D11_VIEWPORT viewport = {};

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	pContext->RSSetViewports(1u, &viewport);

	D3D11_INPUT_ELEMENT_DESC layout[2] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	if (!vertexShader.init(pDevice, L"VertexShader.cso", layout, numElements))
	{
		DisplayError(L"Vertex shader failed to create");
		return false;
	}

	if (!pixelShader.init(pDevice, L"PixelShader.cso"))
	{
		DisplayError(L"Pixel Shader failed to create");
		return false;
	}

	return true;
}

void Graphics::onSize(unsigned int width, unsigned int height)
{
	pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_B8G8R8A8_UNORM, 0);

	HRESULT hr = S_OK;
	ID3D11Texture2D* BackBuffer; //Create our BackBuffer
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBuffer));
	DisplayError(hr, L"GetBuffer failed");
	//Create our Render Target
	hr = pDevice->CreateRenderTargetView(BackBuffer, nullptr, &pTarget);
	DisplayError(hr, L"Create Render Target view failed");
	ReleaseCOM(BackBuffer);

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

}

void Graphics::setFullscreen(bool fullscreen, unsigned int width, unsigned int height)
{
	onSize(width, height);
	pSwapChain->SetFullscreenState(fullscreen, nullptr);
}

void Graphics::setWireframe(bool value)
{
	if (value)
	{
		isWireframeEnabled = true;
		pContext->RSSetState(pWireframeState);
	}
	else
	{
		isWireframeEnabled = false;
		pContext->RSSetState(nullptr);
	}
}

void Graphics::Begin(float r, float g, float b)
{
	float color[4] = { r, g, b, 1.0f };
	pContext->ClearRenderTargetView(pTarget, color);
	pContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Graphics::End()
{
	pSwapChain->Present(1u, 0u);
}
