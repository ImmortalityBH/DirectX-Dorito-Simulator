#include "Graphics/Shader/HullShader.h"
#include "Utility/Error.h"
#include "Utility/d3dUtil.h"

bool HullShader::init(ID3D11Device* pDevice, std::wstring& filePath)
{
	try
	{
		HRESULT hr = S_OK;

		hr = D3DReadFileToBlob(filePath.c_str(), &pBlob);
		std::wstring errMsg = L"Pixel shader could not be loaded at: ";
		errMsg += filePath;
		THROW_IF_FAILED(hr, wstring_to_string(errMsg));

		hr = pDevice->CreateHullShader(pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(), nullptr, &pHullShader);
		THROW_IF_FAILED(hr, "Pixel shader failed to create");
	}
	catch (HrException& e)
	{
		ErrorLogger::Log(e);
		return false;
	}
	return true;
}

HullShader::~HullShader()
{
	ReleaseCOM(pHullShader);
}
