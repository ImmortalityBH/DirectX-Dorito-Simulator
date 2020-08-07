#pragma once

#include <d3d11.h>
#include <string>

class DomainShader
{
public:
	bool init(ID3D11Device* pDevice, std::wstring& filePath);
	DomainShader() = default;
	~DomainShader();

	ID3D11DomainShader* getHullShader() const { return pDomainShader; }
	ID3D10Blob* getBlob() const { return pBlob; }
private:
	ID3D11DomainShader* pDomainShader = nullptr;
	ID3D10Blob* pBlob = nullptr;
};

