#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include "Utility/d3dUtil.h"

class Shader //virutal class
{
public:
	~Shader() { ReleaseCOM(pBlob); }
	virtual ID3DBlob* getBlob() const { return pBlob; }
protected:
	ID3D10Blob* pBlob = nullptr;
};

