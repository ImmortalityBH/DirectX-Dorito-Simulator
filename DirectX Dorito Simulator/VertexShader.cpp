#include "VertexShader.h"
#include "Error.h"
#include "d3dUtil.h"

bool VertexShader::init(ID3D11Device* pDevice, std::wstring filePath, D3D11_INPUT_ELEMENT_DESC* pLayoutDesc, UINT numElements)
{
	HRESULT hr = S_OK;
	hr = D3DReadFileToBlob(filePath.c_str(), &pVertexBlob);
	if (FAILED(hr))
	{
		std::wstring errMsg = L"Pixel shader could not be loaded at: ";
		errMsg += filePath;
		DisplayError(errMsg.c_str());
		return false;
	}

	hr = pDevice->CreateVertexShader(pVertexBlob->GetBufferPointer(),
		pVertexBlob->GetBufferSize(), nullptr, &pVertexShader);
	if (FAILED(hr))
	{
		DisplayError(hr, L"Vertex shader failed to create");
		return false;
	}
	
	hr = pDevice->CreateInputLayout(pLayoutDesc, numElements, pVertexBlob->GetBufferPointer(), 
		pVertexBlob->GetBufferSize(), &pInputLayout);
	if (FAILED(hr))
	{
		DisplayError(L"Could not create input layout");
		false;
	}

	return true;
}

VertexShader::~VertexShader()
{
	ReleaseCOM(pVertexShader);
	ReleaseCOM(pVertexBlob);
	ReleaseCOM(pInputLayout);
}
