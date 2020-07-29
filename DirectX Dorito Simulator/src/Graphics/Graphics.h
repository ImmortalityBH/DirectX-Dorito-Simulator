#pragma once

#include "Win.h"
#include <d3d11.h>
#include <DirectXMath.h>

#include "Graphics/Vertex.h"
#include "Graphics/VertexShader.h"
#include "Graphics/PixelShader.h"
#include "Utility/d3dUtil.h"

class Graphics
{
public:
	Graphics() = default;
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	bool init(unsigned int width, unsigned int height, HWND hWnd);

	void onSize(unsigned int width, unsigned int height);
	void setFullscreen(bool fullscreen, unsigned int widhth, unsigned int height);
	bool getFullscreen() const { return isFullscreen; }
	void setWireframe(bool value);
	bool isWireframe() const { return isWireframeEnabled; }
	void Begin(float r, float g, float b);
	void End();

	ID3D11Device* getDevice() { return pDevice; }
	ID3D11DeviceContext* getContext() { return pContext; }

	VertexShader vertexShader;
	PixelShader pixelShader;
private:
	bool isFullscreen = false;
	bool isWireframeEnabled = false;
	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;
	ID3D11DepthStencilView* pDepthStencilView = nullptr;
	ID3D11RasterizerState* pWireframeState = nullptr;
	ID3D11Texture2D* pDepthStencilBuffer = nullptr;
};

