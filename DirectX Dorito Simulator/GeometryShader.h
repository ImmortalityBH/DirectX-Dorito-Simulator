#pragma once

#include <d3d11.h>
#include <string>

class GeometryShader
{
public:
	bool init(ID3D11Device* pDevice, std::wstring& filePath);
	GeometryShader() = default;
	~GeometryShader();

	ID3D11GeometryShader* getGeometryShader() const { return pGeometryShader; }
	ID3D10Blob* getBlob() const { return pBlob; }
private:
	ID3D11GeometryShader* pGeometryShader = nullptr;
	ID3D10Blob* pBlob = nullptr;
};

