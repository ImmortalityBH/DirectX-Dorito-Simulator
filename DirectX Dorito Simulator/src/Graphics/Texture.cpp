#include "Graphics/Texture.h"
#include <DDSTextureLoader.h>
#include "Utility/Error.h"
#include "Utility/d3dUtil.h"

using namespace DirectX;

Texture::~Texture()
{
	ReleaseCOM(pTexture);
	//ReleaseCOM(pTexSamplerState);
}

bool Texture::load(ID3D11Device* pDevice, std::wstring filePath)
{
	try
	{
		HRESULT hr = S_OK;

		hr = CreateDDSTextureFromFile(pDevice, filePath.c_str(), nullptr, &pTexture);
		THROW_IF_FAILED(hr, "Texture failed to load");

		/*D3D11_SAMPLER_DESC tsd = {};
		tsd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		tsd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		tsd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		tsd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		tsd.ComparisonFunc = D3D11_COMPARISON_NEVER;
		tsd.MinLOD = 0;
		tsd.MaxLOD = D3D11_FLOAT32_MAX;

		hr = pDevice->CreateSamplerState(&tsd, &pTexSamplerState);
		THROW_IF_FAILED(hr, "Create sampler state failed");*/
	}
	catch (HrException& e)
	{
		ErrorLogger::Log(e);
		return false;
	}
	return true;
}
