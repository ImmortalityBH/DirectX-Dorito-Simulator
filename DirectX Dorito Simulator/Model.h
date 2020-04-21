#pragma once

#include "Win.h"
#include "Window.h"
#include "Graphics.h"
#include "WVP.h"

class Model
{
public:
	Model(Graphics& gfx, LPCWSTR texFilename);
	~Model();

	void create();
	void update();
	void draw();

	void bindShaders();
	void unbindShaders();
private:
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pConstantBuffer;

	ID3D11ShaderResourceView* pTexture;
	ID3D11SamplerState* pTexSamplerState;
	LPCWSTR texFilename;

	ID3D11InputLayout* pVertexLayout;
	ID3D11VertexShader* pVertexShader;
	ID3D11PixelShader* pPixelShader;
private:
	DirectX::XMMATRIX scale;
	DirectX::XMMATRIX rotation;
	DirectX::XMMATRIX translation;
	DirectX::XMMATRIX transform;
	ConstantBuffer cb;

	Graphics* pGfx;
};

