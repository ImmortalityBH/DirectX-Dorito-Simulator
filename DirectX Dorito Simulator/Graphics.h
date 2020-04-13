#pragma once

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include "Win.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

struct Vertex
{
	Vertex(float x, float y, float z,
		float r, float g, float b)
		: x(x), y(y), z(z),
		r(r), g(g), b(b) { }
	float x, y, z;
	float r, g, b;
};

class Graphics
{
public:
	Graphics() = default;
	Graphics(int width, int height, HWND hWnd);
	~Graphics();

	ID3D11VertexShader* createVertexShader(LPCWSTR fileName);
	ID3D11PixelShader* createPixelShader(LPCWSTR fileName);

	void Begin(float r, float g, float b);
	void End();

	ID3D11Device* getDevice() { return pDevice; }
	ID3D11DeviceContext* getContext() { return pContext; }

private:
	IDXGISwapChain* pSwap;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pContext;
	ID3D11RenderTargetView* pTarget;
};

