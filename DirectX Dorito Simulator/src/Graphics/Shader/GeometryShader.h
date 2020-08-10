#pragma once

#include <d3d11.h>
#include "Graphics/Shader/Shader.h"

class GeometryShader : public Shader
{
public:
	bool init(ID3D11Device* pDevice, std::wstring& filePath);
	GeometryShader() = default;
	~GeometryShader();

	ID3D11GeometryShader* getGeometryShader() const { return pGeometryShader; }
private:
	ID3D11GeometryShader* pGeometryShader = nullptr;
};

