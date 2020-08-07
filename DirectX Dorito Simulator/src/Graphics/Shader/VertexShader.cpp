#include "Graphics/VertexShader.h"
#include "Utility/Error.h"
#include "Utility/d3dUtil.h"

bool VertexShader::init(ID3D11Device* pDevice, std::wstring filePath, D3D11_INPUT_ELEMENT_DESC* pLayoutDesc, UINT numElements)
{
	try
	{
		HRESULT hr = S_OK;
		hr = D3DReadFileToBlob(filePath.c_str(), &pVertexBlob);
		std::wstring errMsg = L"Vertex shader could not be loaded at: ";
		errMsg += filePath;
		THROW_IF_FAILED(hr, wstring_to_string(errMsg));

		hr = pDevice->CreateVertexShader(pVertexBlob->GetBufferPointer(),
			pVertexBlob->GetBufferSize(), nullptr, &pVertexShader);
		THROW_IF_FAILED(hr, "Could not create Vertex shader");

		hr = pDevice->CreateInputLayout(pLayoutDesc, numElements, pVertexBlob->GetBufferPointer(),
			pVertexBlob->GetBufferSize(), &pInputLayout);
		THROW_IF_FAILED(hr, "Could not create input layout");
	}
	catch (BrianException& e)
	{
		ErrorLogger::Log(e);
		return false;
	}
	return true;
}

VertexShader::~VertexShader()
{
	ReleaseCOM(pVertexShader);
	ReleaseCOM(pVertexBlob);
	ReleaseCOM(pInputLayout);
}
