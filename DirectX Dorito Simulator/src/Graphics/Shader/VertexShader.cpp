#include "Graphics/Shader/VertexShader.h"
#include "Utility/Error.h"
#include "Utility/d3dUtil.h"

bool VertexShader::init(ID3D11Device* pDevice, std::wstring filePath, D3D11_INPUT_ELEMENT_DESC* pLayoutDesc, UINT numElements)
{
	try
	{
		HRESULT hr = S_OK;
		hr = D3DReadFileToBlob(filePath.c_str(), &pBlob);
		std::wstring errMsg = L"Vertex shader could not be loaded at: ";
		errMsg += filePath;
		THROW_IF_FAILED(hr, wstring_to_string(errMsg));

		hr = pDevice->CreateVertexShader(pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(), nullptr, &pVertexShader);
		THROW_IF_FAILED(hr, "Could not create Vertex shader");

		hr = pDevice->CreateInputLayout(pLayoutDesc, numElements, pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(), &pInputLayout);
		THROW_IF_FAILED(hr, "Could not create input layout");
	}
	catch (HrException& e)
	{
		ErrorLogger::Log(e);
		return false;
	}
	return true;
}

VertexShader::~VertexShader()
{
	ReleaseCOM(pVertexShader);
	ReleaseCOM(pInputLayout);
}
