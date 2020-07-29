#include "Graphics/CubemapTexture.h"
#include <DDSTextureLoader.h>
#include "Utility/Error.h"
#include "Utility/d3dUtil.h"

using namespace DirectX;

CubemapTexture::~CubemapTexture()
{
	ReleaseCOM(pTexture);
}

bool CubemapTexture::load(ID3D11Device* pDevice, std::vector<std::wstring>& filePath)
{
	HRESULT hr = S_OK;

	for (std::size_t i = 0; i < numOfSides; i++)
	{
		CreateDDSTextureFromFile(pDevice, filePath[i].c_str(),
			reinterpret_cast<ID3D11Resource**>(pTexArray[i]), nullptr);
	}

	return true;
}
