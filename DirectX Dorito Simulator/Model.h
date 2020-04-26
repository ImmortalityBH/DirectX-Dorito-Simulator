#pragma once

#include "Win.h"
#include "Window.h"
#include "Graphics.h"
#include "WVP.h"

#include <vector>

class Model
{
public:
	Model(Graphics& gfx, LPCWSTR texFilename);
	~Model();

	void create(std::vector<Vertex>& vertices, 
		LPCWSTR vertexFileName, LPCWSTR pixelFileName);
	void create(std::vector<Vertex>& vertices, std::vector<UINT>& indices, 
		LPCWSTR vertexFileName, LPCWSTR pixelFileName);
	void update();
	void draw();

	void bind();
	void unbind();
private:
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;
	ID3D11Buffer* pConstantBuffer;

	ID3D11ShaderResourceView* pTexture;
	ID3D11SamplerState* pTexSamplerState;
	LPCWSTR texFilename;

	ID3D11InputLayout* pVertexLayout;
	ID3D11VertexShader* pVertexShader;
	ID3D11PixelShader* pPixelShader;
	ID3DBlob* pVertexBlob;
	ID3DBlob* pPixelBlob;
private:
	DirectX::XMMATRIX scale;
	DirectX::XMMATRIX rotation;
	DirectX::XMMATRIX translation;
	DirectX::XMMATRIX transform;
	ConstantBuffer cb;

	Graphics* pGfx;

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0;
	const D3D11_INPUT_ELEMENT_DESC layout[2] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT vertexCount;
	UINT indexCount;
};

