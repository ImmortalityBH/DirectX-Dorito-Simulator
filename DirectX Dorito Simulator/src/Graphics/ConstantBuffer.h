#pragma once

#include <d3d11.h>
#include "Utility/Error.h"


////////ConstantBuffer types
struct CB_WVP {
	DirectX::XMMATRIX WVP;
};

struct CB_SKYBOX {
	DirectX::XMMATRIX bruh;
};

template<class T>
class ConstantBuffer
{
public:
	ConstantBuffer() = default;
	ConstantBuffer(const ConstantBuffer<T>&) = delete;
	ConstantBuffer& operator=(const ConstantBuffer<T>&) = delete;
	~ConstantBuffer()
	{
		ReleaseCOM(pBuffer);
	}	
	T data;

	HRESULT init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	{
		if (pBuffer != nullptr)
			ReleaseCOM(pBuffer);

		this->pContext = pContext;

		D3D11_BUFFER_DESC cbd = {};
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0;
		cbd.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
		cbd.StructureByteStride = 0;

		HRESULT hr = pDevice->CreateBuffer(&cbd, 0, &pBuffer);
		return hr;
	}
	bool update()
	{
		D3D11_MAPPED_SUBRESOURCE msr = {};
		HRESULT hr = pContext->Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		if (FAILED(hr)) {
			ErrorLogger::Log(hr, L"Failed to map constant buffer.");
			return false;
		}
		CopyMemory(msr.pData, &data, sizeof(T));
		pContext->Unmap(pBuffer, 0);
		return true;
	}
public://getter
	ID3D11Buffer* const* getBuffer() const { return &pBuffer; }
private:
	ID3D11Buffer* pBuffer = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
};

/*#include <d3d11.h>
#include <DirectXMath.h>
#include "Utility/d3dUtil.h"

template<class T>
class ConstantBuffer
{
private:
	ID3D11Buffer* pBuffer = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
public:
	ConstantBuffer() = default;
	ConstantBuffer(const ConstantBuffer<T>&) = delete;
	ConstantBuffer& operator=(const ConstantBuffer<T>&) = delete;
	~ConstantBuffer()
	{
		ReleaseCOM(pBuffer);
	}

	T data;

	HRESULT init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	{
		this->pContext = pContext;

		D3D11_BUFFER_DESC cbd = {};

		cbd.ByteWidth = sizeof(T);
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = T;

		HRESULT hr = pDevice->CreateBuffer(&cbd, &sd, &pBuffer);
		return hr;
	}
	bool Update()
	{
		//pGfx->getContext()->UpdateSubresource(pConstantBuffer, 0, NULL, &cb, 0, 0);
		D3D11_MAPPED_SUBRESOURCE msr = {};
		HRESULT hr = pContext->Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr));
		//if (FAILED(hr))
			//return false;
		//CopyMemory(msr.pData, &data, sizeof(T));

		pContext->Unmap(pBuffer, 0);
		return true;
	}
public://getters
	ID3D11Buffer* getBuffer() const { return pBuffer; }
	//bool mapped = false;
};*/
