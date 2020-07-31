#pragma once

#include <d3d11.h>

class IndexBuffer
{
public:
	IndexBuffer() = default;
	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;

	HRESULT init(ID3D11Device* pDevice, DWORD* data, UINT numIndices);
public://getters
	ID3D11Buffer* getBuffer() const { return pBuffer; }
	const UINT getIndexCount() const { return indexCount; }
	const UINT* getIndexCountPtr() const { return &indexCount; }
private:
	ID3D11Buffer* pBuffer = nullptr;
	UINT indexCount = 0;
};

