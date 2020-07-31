#include "VertexBuffer.h"
#include "Utility/d3dUtil.h"

template<class T>
inline VertexBuffer<T>::~VertexBuffer()
{
	ReleaseCOM(pBuffer);
}

template<class T>
inline HRESULT VertexBuffer<T>::init(ID3D11Device* pDevice, T* data, UINT numVertices)
{
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