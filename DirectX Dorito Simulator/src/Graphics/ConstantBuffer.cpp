#include "ConstantBuffer.h"

template<class T>
inline HRESULT ConstantBuffer<T>::init(ID3D11Device* pDevice, T* data, bool mapped)
{
	D3D11_BUFFER_DESC cbd = {};
	this->mapped = mapped;
	if (mapped)
	{
		cbd.ByteWidth = sizeof(T);
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.StructureByteStride = 0u;
	}
	else
	{
		cbd.ByteWidth = sizeof(T);
		cbd.Usage = D3D11_USAGE_DEFAULT;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = 0u;
		cbd.MiscFlags = 0u;
		cbd.StructureByteStride = 0u;
	}
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = T;

	HRESULT hr = pDevice->CreateBuffer(&cbd, &sd, &pBuffer);
	return hr;
}