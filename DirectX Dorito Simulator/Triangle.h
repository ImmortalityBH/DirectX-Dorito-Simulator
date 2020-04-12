#pragma once

#include "Graphics.h"

class Triangle
{
public:
	Triangle(Graphics& gfx);
	~Triangle();

	void Create();
	void Draw();

private:
	ID3D11Buffer* pVertexBuffer;
	ID3D11VertexShader* pVertexShader;
	ID3D11PixelShader* pPixelShader;
	Graphics* pGfx;
};

