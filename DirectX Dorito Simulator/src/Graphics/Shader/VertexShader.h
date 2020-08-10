#pragma once

#include <d3d11.h>
#include "Graphics/Shader/Shader.h"

class VertexShader : public Shader
{
public:
	bool init(ID3D11Device* pDevice, std::wstring filePath, 
		D3D11_INPUT_ELEMENT_DESC* pLayoutDesc, UINT numElements);
	VertexShader() = default;
	~VertexShader();

	ID3D11VertexShader* getVertexShader() const { return pVertexShader; }
	ID3D11InputLayout* getInputLayout() const{ return pInputLayout; }
private:
	ID3D11VertexShader* pVertexShader = nullptr;
	ID3D11InputLayout* pInputLayout = nullptr;
};

