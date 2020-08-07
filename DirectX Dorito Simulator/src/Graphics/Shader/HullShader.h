#pragma once

#include <d3d11.h>
#include <string>

class HullShader
{
public:
	bool init(ID3D11Device* pDevice, std::wstring& filePath);
	HullShader() = default;
	~HullShader();

	ID3D11HullShader* getHullShader() const { return pHullShader; }
	ID3D10Blob* getBlob() const { return pBlob; }
private:
	ID3D11HullShader* pHullShader = nullptr;
	ID3D10Blob* pBlob = nullptr;
};

