#include "Graphics/Shader/DomainShader.h"
#include "Utility/Error.h"
#include "Utility/d3dUtil.h"

bool DomainShader::init(ID3D11Device* pDevice, std::wstring& filePath)
{
	try
	{
		HRESULT hr = S_OK;

		hr = D3DReadFileToBlob(filePath.c_str(), &pBlob);
		std::wstring errMsg = L"Pixel shader could not be loaded at: ";
		errMsg += filePath;
		THROW_IF_FAILED(hr, wstring_to_string(errMsg));

		hr = pDevice->CreateDomainShader(pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(), nullptr, &pDomainShader);
		THROW_IF_FAILED(hr, "Pixel shader failed to create");
	}
	catch (BrianException & e)
	{
		ErrorLogger::Log(e);
		return false;
	}
	return true;
}

DomainShader::~DomainShader()
{
	ReleaseCOM(pDomainShader);
}
