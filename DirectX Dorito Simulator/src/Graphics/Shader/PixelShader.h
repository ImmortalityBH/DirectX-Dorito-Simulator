#pragma once

#include <d3d11.h>
#include "Graphics/Shader/Shader.h"

class PixelShader : public Shader
{
public:
	bool init(ID3D11Device* pDevice, std::wstring filePath);
	PixelShader() = default;
	~PixelShader();

	ID3D11PixelShader* getPixelShader() const { return pPixelShader; }
private:
	ID3D11PixelShader* pPixelShader = nullptr;
};
