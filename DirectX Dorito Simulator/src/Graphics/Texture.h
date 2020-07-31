#pragma once

#include <d3d11.h>
#include <string>

class Texture
{
public:
	Texture() = default;
	~Texture();

	bool load(ID3D11Device* pDevice, std::wstring filePath);
	//return double ptr to objs for pssetblahblah functions
	ID3D11ShaderResourceView* const* getTexture() const { return &pTexture; }
	//ID3D11SamplerState** getSamplerState() { return &pTexSamplerState; }
private:
	ID3D11ShaderResourceView* pTexture = nullptr;
	//ID3D11SamplerState* pTexSamplerState = nullptr;
};

