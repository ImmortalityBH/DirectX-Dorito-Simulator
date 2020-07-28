#include "PixelShader.h"
#include "Error.h"
#include "d3dUtil.h"

bool PixelShader::init(ID3D11Device* pDevice, std::wstring filePath)
{
	HRESULT hr = S_OK;

	hr = D3DReadFileToBlob(filePath.c_str(), &pPixelBlob);
	if (FAILED(hr))
	{
		std::wstring errMsg = L"Pixel shader could not be loaded at: ";
		errMsg += filePath;
		DisplayError(errMsg.c_str());
		return false;
	}
	
	hr = pDevice->CreatePixelShader(pPixelBlob->GetBufferPointer(),
		pPixelBlob->GetBufferSize(), nullptr, &pPixelShader);
	if (FAILED(hr))
	{
		DisplayError(hr, L"Pixel shader failed to create");
		return false;
	}

	return true;
}

PixelShader::~PixelShader()
{
	ReleaseCOM(pPixelShader);
	ReleaseCOM(pPixelBlob);
}
