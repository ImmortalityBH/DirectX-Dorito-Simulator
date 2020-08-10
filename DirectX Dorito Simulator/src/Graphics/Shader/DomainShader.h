#pragma once

#include <d3d11.h>
#include "Graphics/Shader/Shader.h"

class DomainShader : public Shader
{
public:
	bool init(ID3D11Device* pDevice, std::wstring& filePath);
	DomainShader() = default;
	~DomainShader();

	ID3D11DomainShader* getHullShader() const { return pDomainShader; }
private:
	ID3D11DomainShader* pDomainShader = nullptr;
};

