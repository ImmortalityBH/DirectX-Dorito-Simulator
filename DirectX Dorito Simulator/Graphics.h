#pragma once

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include "Win.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "VertexShader.h"
#include "PixelShader.h"

struct Vertex
{
	Vertex(float x, float y, float z,
		float u, float v)
		: pos(x, y, z), uv(u, v) {}
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

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
	void setWireframe(bool value);
	void Begin(float r, float g, float b);
	void End();

	ID3D11Device* getDevice() { return pDevice; }
	ID3D11DeviceContext* getContext() { return pContext; }

	VertexShader vertexShader;
	PixelShader pixelShader;
private:
	IDXGISwapChain* pSwapChain;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pContext;
	ID3D11RenderTargetView* pTarget;
	ID3D11DepthStencilView* pDepthStencilView;
	ID3D11RasterizerState* pWireframeState;
	ID3D11Texture2D* pDepthStencilBuffer;
};

