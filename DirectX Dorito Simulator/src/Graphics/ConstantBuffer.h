#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

struct CB_WVP {
	DirectX::XMMATRIX WVP;
};

template<class T>
class ConstantBuffer 
{
public:
	ConstantBuffer() = default;
	ConstantBuffer(const ConstantBuffer&) = delete;
	ConstantBuffer& operator=(const ConstantBuffer&) = delete;

	HRESULT init(ID3D11Device* pDevice, T* data, bool mapped);

	ID3D11Buffer* getBuffer() const { return pBuffer; }
private:
	ID3D11Buffer* pBuffer = nullptr;
	bool mapped = false;
};
