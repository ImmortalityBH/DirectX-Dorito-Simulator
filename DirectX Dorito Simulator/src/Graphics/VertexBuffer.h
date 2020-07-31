#pragma once

#include <d3d11.h>
#include "Utility/d3dUtil.h"

template<class T>
class VertexBuffer
{
public:
	VertexBuffer() = default;
	~VertexBuffer()
	{
		ReleaseCOM(pBuffer);
	}
	VertexBuffer(const VertexBuffer<T>&) = delete;
	VertexBuffer& operator=(const VertexBuffer<T>&) = delete;

	HRESULT init(ID3D11Device* pDevice, T* data, UINT numVertices)
	{
		if (pBuffer != nullptr)
			ReleaseCOM(pBuffer);
		this->vertexCount = numVertices;
		this->stride = sizeof(T);

		D3D11_BUFFER_DESC vbd = {};//fill out buffer desc of type vertex
		vbd.ByteWidth = sizeof(T) * numVertices;
		vbd.Usage = D3D11_USAGE_DEFAULT;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA sd = {};//give sub data to buffer upon creation
		sd.pSysMem = data;

		HRESULT hr = pDevice->CreateBuffer(&vbd, &sd, &pBuffer);
		return hr;//handle exceptions later
	}
public://getters
	ID3D11Buffer* const* getBuffer() { return &pBuffer; }
	const UINT getStride() const { return stride; }
	const UINT* getStridePtr() const { return &stride; }
	const UINT getVertexCount() const { return vertexCount; }
private:
	ID3D11Buffer* pBuffer = nullptr;
	UINT stride = 0;
	UINT vertexCount = 0;
};
