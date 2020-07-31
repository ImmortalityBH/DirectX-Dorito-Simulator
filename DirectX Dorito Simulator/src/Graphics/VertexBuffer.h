#pragma once

#include <d3d11.h>

template<class T>
class VertexBuffer
{
public:
	VertexBuffer() = default;
	~VertexBuffer();
	VertexBuffer(const VertexBuffer<T>&) = delete;
	VertexBuffer& operator=(const VertexBuffer<T>&) = delete;

	HRESULT init(ID3D11Device* pDevice, T* data, UINT numVertices);
public://getters
	ID3D11Buffer* getBuffer() const { return pBuffer; }
	const UINT getStride() const { return stride; }
	const UINT* getStridePtr() const { return &stride; }
	const UINT getBufferSize() const { return bufferSize; }
private:
	ID3D11Buffer* pBuffer = nullptr;
	UINT stride = 0;
	UINT vertexCount = 0;
};
