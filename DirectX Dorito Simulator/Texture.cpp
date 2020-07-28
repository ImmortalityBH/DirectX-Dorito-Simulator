#include "Texture.h"
#include <DDSTextureLoader.h>
#include "Error.h"
#include "d3dUtil.h"

using namespace DirectX;

Texture::~Texture()
{
	ReleaseCOM(pTexture);
	ReleaseCOM(pTexSamplerState);
}

bool Texture::load(ID3D11Device* pDevice, std::wstring filePath)
{
	HRESULT hr = S_OK;

	hr = CreateDDSTextureFromFile(pDevice, filePath.c_str(), nullptr, &pTexture);
	if (FAILED(hr))
	{
		DisplayError(L"Texture failed to load!");
		return false;
	}
	
	D3D11_SAMPLER_DESC tsd = {};
	tsd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	tsd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	tsd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	tsd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	tsd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	tsd.MinLOD = 0;
	tsd.MaxLOD = D3D11_FLOAT32_MAX;

	hr = pDevice->CreateSamplerState(&tsd, &pTexSamplerState);
	if (FAILED(hr))
	{
		DisplayError(L"Create sampler state failed.");
		return false;
	}

	return true;
}
