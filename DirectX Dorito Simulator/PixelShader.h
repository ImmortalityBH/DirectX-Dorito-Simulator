#pragma once

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>

class PixelShader
{
public:
	bool init(ID3D11Device* pDevice, std::wstring filePath);
	PixelShader() = default;
	~PixelShader();

	ID3D11PixelShader* getPixelShader() const
	{
		return pPixelShader;
	}
	ID3D10Blob* getPixelBlob() const
	{
		return pPixelBlob;
	}
private:
	ID3D11PixelShader* pPixelShader;
	ID3D10Blob* pPixelBlob;
};
