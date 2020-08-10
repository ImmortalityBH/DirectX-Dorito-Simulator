#include "Graphics/Graphics.h"
#include "Utility/Error.h"
#include "Utility/d3dUtil.h"
//#include "imgui/imgui_impl_dx11.h"

using namespace DirectX;

Graphics::~Graphics()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	ReleaseCOM(pSwapChain);
	ReleaseCOM(pDevice);
	ReleaseCOM(pContext);
	ReleaseCOM(pRenderTarget);
	ReleaseCOM(pDepthStencilBuffer);
	ReleaseCOM(pDepthStencilView);
	ReleaseCOM(pWireframeState);
	ReleaseCOM(pTexSamplerState);
}

bool Graphics::init(bool isFullscreen, bool isVsync, unsigned int width, unsigned int height, HWND hWnd)
{
	try
	{
		this->isFullscreen = isFullscreen;
		this->isVsync = isVsync;
		this->samplingLevel = LEVEL_4;
		HRESULT hr = S_OK;

		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	#if defined(_DEBUG)
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif
		D3D_FEATURE_LEVEL feature_level;

		hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE, 
			nullptr,
			creationFlags, 
			nullptr, 
			NULL,
			D3D11_SDK_VERSION,
			&pDevice, 
			&feature_level,
			&pContext
		);
		THROW_IF_FAILED(hr, "Create device and swapchain failed");
		if (feature_level < D3D_FEATURE_LEVEL_11_0) {
			THROW_NORMAL("You must have DirectX 11 Compatible graphics card!");
		}
		hr = pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, samplingLevel, &sampleQuality);
		if (!(sampleQuality > 0)) {
			THROW_NORMAL("Sampling level not supported");
		}

		//Create SwapChain
		DXGI_SWAP_CHAIN_DESC sd = {};

		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.SampleDesc.Count = samplingLevel;
		sd.SampleDesc.Quality = sampleQuality - 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = hWnd;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		IDXGIDevice* dxgiDevice = nullptr;
		hr = pDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		THROW_IF_FAILED(hr, "BRO");
		IDXGIAdapter* dxgiAdapter = nullptr;
		dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter));

		IDXGIFactory* dxgiFactory = nullptr;
		dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));
		THROW_IF_FAILED(hr, "Facotry");
		dxgiFactory->CreateSwapChain(pDevice, &sd, &pSwapChain);
		ReleaseCOM(dxgiDevice);
		ReleaseCOM(dxgiAdapter);
		ReleaseCOM(dxgiFactory);

		onSize(width, height);
		/*//onSize(width, height);
		ID3D11Texture2D* BackBuffer; //Create our BackBuffer
		hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBuffer));
		THROW_IF_FAILED(hr, "GetBuffer failed");
		//Create our Render Target
		hr = pDevice->CreateRenderTargetView(BackBuffer, nullptr, &pRenderTarget);
		ReleaseCOM(BackBuffer);//release backbuffer incase of exception
		THROW_IF_FAILED(hr, "Create render target view failed");

		D3D11_TEXTURE2D_DESC ds = {};
		ds.Width = width;
		ds.Height = height;
		ds.MipLevels = 1;
		ds.ArraySize = 1;
		ds.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		ds.SampleDesc.Count = samplingLevel;
		ds.SampleDesc.Quality = sampleQuality - 1;
		ds.Usage = D3D11_USAGE_DEFAULT;
		ds.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		ds.CPUAccessFlags = 0;
		ds.MiscFlags = 0;

		hr = pDevice->CreateTexture2D(&ds, nullptr, &pDepthStencilBuffer);
		THROW_IF_FAILED(hr, "DepthStencilBuffer could not be created");
		hr = pDevice->CreateDepthStencilView(pDepthStencilBuffer, nullptr, &pDepthStencilView);
		THROW_IF_FAILED(hr, "DepthStencilView could not be created");
		//Set our Render Target
		pContext->OMSetRenderTargets(1, &pRenderTarget, pDepthStencilView);
		
		//viewport declared with C for constructor and convience 
		CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, 1.0f);
		pContext->RSSetViewports(1u, &viewport);*/

		D3D11_RASTERIZER_DESC rsd = {};
		rsd.FillMode = D3D11_FILL_WIREFRAME;
		rsd.CullMode = D3D11_CULL_NONE;

		hr = pDevice->CreateRasterizerState(&rsd, &pWireframeState);
		THROW_IF_FAILED(hr, "Create rasterizer state failed");

		D3D11_INPUT_ELEMENT_DESC layout[2] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = ARRAYSIZE(layout);

		if (!vertexShader.init(pDevice, L"VertexShader.cso", layout, numElements))
			THROW_NORMAL("Vertex Shader failed to create");
		if (!pixelShader.init(pDevice, L"PixelShader.cso"))
			THROW_NORMAL("Pixel Shader failed to create");

		/*D3D11_INPUT_ELEMENT_DESC layout2[2] = 
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		numElements = ARRAYSIZE(layout);

		if (!vertexShaderColor.init(pDevice, L"VertexShaderColor.cso", layout2, numElements))
			THROW_NORMAL("Vertex Shader failed to create");
		if (!pixelShaderColor.init(pDevice, L"PixelShaderColor.cso"))
			THROW_NORMAL("Pixel Shader failed to create");*/

		//create texture sampler state
		D3D11_SAMPLER_DESC tsd = {};
		tsd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		tsd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		tsd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		tsd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		tsd.ComparisonFunc = D3D11_COMPARISON_NEVER;
		tsd.MinLOD = 0;
		tsd.MaxLOD = D3D11_FLOAT32_MAX;

		hr = pDevice->CreateSamplerState(&tsd, &pTexSamplerState);
		THROW_IF_FAILED(hr, "Create sampler state failed");

		//setup imgui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplWin32_Init(hWnd);
		ImGui_ImplDX11_Init(this->pDevice, this->pContext);
		ImGui::StyleColorsDark();
	}
	catch (HrException& e)
	{
		ErrorLogger::Log(e);
		return false;
	}
	return true;

}

void Graphics::onSize(unsigned int newWidth, unsigned int newHeight)
{
	ReleaseCOM(pRenderTarget);
	ReleaseCOM(pDepthStencilView);
	ReleaseCOM(pDepthStencilBuffer);

	HRESULT hr = S_OK;

	hr = pSwapChain->ResizeBuffers(1, newWidth, newHeight, DXGI_FORMAT_B8G8R8A8_UNORM, 0);
	ID3D11Texture2D* BackBuffer; //Create our BackBuffer
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBuffer));
	THROW_IF_FAILED(hr, "GetBuffer failed");
	//Create our Render Target
	hr = pDevice->CreateRenderTargetView(BackBuffer, nullptr, &pRenderTarget);
	ReleaseCOM(BackBuffer);//release backbuffer incase of exception
	THROW_IF_FAILED(hr, "Create render target view failed");

	D3D11_TEXTURE2D_DESC ds = {};
	ds.Width = newWidth;
	ds.Height = newHeight;
	ds.MipLevels = 1;
	ds.ArraySize = 1;
	ds.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	ds.SampleDesc.Count = samplingLevel;
	ds.SampleDesc.Quality = sampleQuality - 1;
	ds.Usage = D3D11_USAGE_DEFAULT;
	ds.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	ds.CPUAccessFlags = 0;
	ds.MiscFlags = 0;

	hr = pDevice->CreateTexture2D(&ds, nullptr, &pDepthStencilBuffer);
	THROW_IF_FAILED(hr, "DepthStencilBuffer could not be created");
	hr = pDevice->CreateDepthStencilView(pDepthStencilBuffer, nullptr, &pDepthStencilView);
	THROW_IF_FAILED(hr, "DepthStencilView could not be created");
	//Set our Render Target
	pContext->OMSetRenderTargets(1, &pRenderTarget, pDepthStencilView);

	CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(newWidth), static_cast<float>(newHeight), 0.0f, 1.0f);
	pContext->RSSetViewports(1, &viewport);
}

void Graphics::setFullscreen(bool fullscreen, unsigned int width, unsigned int height)
{
	//onSize(width, height);
	this->isFullscreen = fullscreen;
	pSwapChain->SetFullscreenState(fullscreen, nullptr);
}

void Graphics::setWireframe(bool value)
{
	isWireframeEnabled = value;
	if (value)
		pContext->RSSetState(pWireframeState);
	else
		pContext->RSSetState(nullptr);
}

void Graphics::Begin(float r, float g, float b)
{
	float color[4] = { r, g, b, 1.0f };
	pContext->ClearRenderTargetView(pRenderTarget, color);
	pContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	pContext->PSSetSamplers(0u, 1u, &pTexSamplerState);
}

void Graphics::End()
{
	pSwapChain->Present(isVsync, 0u);
}
