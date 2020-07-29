#pragma once

#include <d3d11.h>
#include <string>
#include <vector>
#include "Graphics/Texture.h"

class CubemapTexture
{
public:
	CubemapTexture() = default;
	~CubemapTexture();

	bool load(ID3D11Device* pDevice, std::vector<std::wstring>& filePath);
	//return double ptr to objs for pssetblahblah functions

	ID3D11ShaderResourceView** getTexture() { return &pTexture; }
	//ID3D11SamplerState** getSamplerState() { return &pTexSamplerState; }
private:
	static constexpr std::size_t numOfSides = 6;
	ID3D11ShaderResourceView* pTexture = nullptr;
	ID3D11Texture2D* pTexArray[6];
};
