#pragma once

#include <d3d11.h>
#include "Graphics/Shader/Shader.h"

class HullShader : public Shader
{
public:
	bool init(ID3D11Device* pDevice, std::wstring& filePath);
	HullShader() = default;
	~HullShader();

	ID3D11HullShader* getHullShader() const { return pHullShader; }
private:
	ID3D11HullShader* pHullShader = nullptr;
};

