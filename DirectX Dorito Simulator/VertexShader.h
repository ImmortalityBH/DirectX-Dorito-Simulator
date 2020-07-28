#pragma once

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>

class VertexShader
{
public:
	bool init(ID3D11Device* pDevice, std::wstring filePath, 
		D3D11_INPUT_ELEMENT_DESC* pLayoutDesc, UINT numElements);
	VertexShader() = default;
	~VertexShader();

	ID3D11VertexShader* getVertexShader() const { return pVertexShader; }
	ID3D10Blob* getVertexBlob() const { return pVertexBlob; }
	ID3D11InputLayout* getInputLayout() const{ return pInputLayout; }
private:
	ID3D11VertexShader* pVertexShader;
	ID3D10Blob* pVertexBlob;
	ID3D11InputLayout* pInputLayout;
};

