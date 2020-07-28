#pragma once

#pragma comment (lib, "d3d11.lib")
#include <d3d11.h>
#include <string>


class Texture
{
public:
	Texture() = default;
	~Texture();

	bool load(ID3D11Device* pDevice, std::wstring filePath);

	ID3D11ShaderResourceView* getTexture() const { return pTexture; }
	ID3D11SamplerState* getTexSamplerState() const { return pTexSamplerState; }

private:
	ID3D11ShaderResourceView* pTexture;
	ID3D11SamplerState* pTexSamplerState;
};

