#pragma once

#include "Win.h"
#include <d3d11.h>
#include <DirectXMath.h>

#include "Graphics/Vertex.h"
#include "Graphics/Shader/VertexShader.h"
#include "Graphics/Shader/PixelShader.h"
#include "Utility/d3dUtil.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

class Graphics
{
public:
	enum SAMPLE_LEVEL
	{
		LEVEL_NONE = 1,
		LEVEL_4 = 4,
		LEVEL_8 = 8,
		LEVEL_16 = 16,
	} samplingLevel;
public:
	Graphics() = default;
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	bool init(bool isFullscreen, bool isVsync, unsigned int width, unsigned int height, HWND hWnd);

	void onSize(unsigned int newWidth, unsigned int newHeight);
	void setFullscreen(bool fullscreen, unsigned int widhth, unsigned int height);
	bool getFullscreen() const { return isFullscreen; }
	void setWireframe(bool value);
	bool isWireframe() const { return isWireframeEnabled; }
	void Begin(float r, float g, float b);
	void End();

	ID3D11Device* getDevice() { return pDevice; }
	ID3D11DeviceContext* getContext() { return pContext; }

	ID3D11SamplerState* const* getSamplerState() { return &pTexSamplerState; }
public:
	VertexShader vertexShader;
	PixelShader pixelShader;
	//VertexShader vertexShaderColor;
	//PixelShader pixelShaderColor;
	//VertexShader vertexShaderSkybox;
	//PixelShader pixelShaderSkybox;
private:
	bool isVsync = false;
	bool isFullscreen = false;
	bool isWireframeEnabled = false;
	UINT sampleQuality = 0;
	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pRenderTarget = nullptr;
	ID3D11DepthStencilView* pDepthStencilView = nullptr;
	ID3D11RasterizerState* pWireframeState = nullptr;
	ID3D11Texture2D* pDepthStencilBuffer = nullptr;
	ID3D11SamplerState* pTexSamplerState = nullptr;
};

