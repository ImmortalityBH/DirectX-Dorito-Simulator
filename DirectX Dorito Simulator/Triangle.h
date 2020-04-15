#pragma once

#include "Graphics.h"

struct ConstantBuffer
{
	DirectX::XMMATRIX transform;
};

class Triangle
{
public:
	Triangle(Graphics& gfx);
	~Triangle();

	void Create();
	void Update();
	void Draw();

	void Scale(float x, float y, float z);
	void Rotate(float angle);
	void Move(float x, float y);

private:
	ID3D11Buffer* pVertexBuffer;

	ID3D11Buffer* pConstantBuffer;
	ConstantBuffer cb;

	ID3D11InputLayout* pVertexLayout;
	ID3D11VertexShader* pVertexShader;
	ID3D11PixelShader* pPixelShader;
	Graphics* pGfx;

	DirectX::XMMATRIX scale;
	DirectX::XMMATRIX rotation;
	DirectX::XMMATRIX translation;
	DirectX::XMMATRIX transform;
}; 

