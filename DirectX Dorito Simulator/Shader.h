#pragma once

#include <d3d11.h>

//virutal class

class Shader
{
public:
	ID3DBlob* getBlob() const { return pBlob; }
private:
	ID3D10Blob* pBlob = nullptr;
};

