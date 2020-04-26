#include "Model.h"
#include "Error.h"
#include <DDSTextureLoader.h>

using namespace DirectX;

Model::Model(Graphics& gfx, LPCWSTR texFilename)
	: pGfx(&gfx), texFilename(texFilename)
{
}

Model::~Model()
{
	if (pVertexBuffer) pVertexBuffer->Release();
	if (pConstantBuffer) pConstantBuffer->Release();
	if (pTexture) pTexture->Release();
	if (pTexSamplerState) pTexSamplerState->Release();
	if (pVertexLayout) pVertexLayout->Release();
	if (pVertexShader) pVertexShader->Release();
	if (pPixelShader) pPixelShader->Release();
	if (pIndexBuffer) pIndexBuffer->Release();
}

void Model::create(std::vector<Vertex>& vertices, LPCWSTR vertexFileName, LPCWSTR pixelFileName)
{
	HRESULT hr;
	//res / shader / SHOULD BE THIS UPON RLEASE
	//
	//
	hr = D3DReadFileToBlob(vertexFileName, &pVertexBlob);
	DisplayError(hr, L"Vertex shader failed to load");
	hr = D3DReadFileToBlob(pixelFileName, &pPixelBlob);
	DisplayError(hr, L"Pixel shader failed to load");
	//Create the Shader Objects
	hr = pGfx->getDevice()->CreateVertexShader(pVertexBlob->GetBufferPointer(),
		pVertexBlob->GetBufferSize(), nullptr, &pVertexShader);
	DisplayError(hr, L"Vertex shader failed to create");
	hr = pGfx->getDevice()->CreatePixelShader(pPixelBlob->GetBufferPointer(),
		pPixelBlob->GetBufferSize(), nullptr, &pPixelShader);
	DisplayError(hr, L"Pixel shader failed to create");

	vertexCount = vertices.size();
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices.data();

	hr = pGfx->getDevice()->CreateBuffer(&bd, &sd, &pVertexBuffer);
	DisplayError(hr, L"Create Buffer failed");

	hr = pGfx->getDevice()->CreateInputLayout(layout, std::size(layout), pVertexBlob->GetBufferPointer(),
		pVertexBlob->GetBufferSize(), &pVertexLayout);
	DisplayError(hr, L"Create Input layout failed");

	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.CPUAccessFlags = 0;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(ConstantBuffer);

	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	pGfx->getDevice()->CreateBuffer(&cbd, &csd, &pConstantBuffer);
	DisplayError(hr, L"Create Buffer failed");

	hr = CreateDDSTextureFromFile(pGfx->getDevice(), texFilename, nullptr, &pTexture);
	DisplayError(hr, L"Texture failed to load!");

	D3D11_SAMPLER_DESC tsd = {};
	tsd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	tsd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	tsd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	tsd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	tsd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	tsd.MinLOD = 0;
	tsd.MaxLOD = D3D11_FLOAT32_MAX;

	hr = pGfx->getDevice()->CreateSamplerState(&tsd, &pTexSamplerState);
	DisplayError(hr, L"Create sampler state failed.");

	if (pVertexBlob) pVertexBlob->Release();
	if (pPixelBlob) pPixelBlob->Release();
}

void Model::create(std::vector<Vertex>& vertices, std::vector<UINT>& indices, LPCWSTR vertexFileName, LPCWSTR pixelFileName)
{
	HRESULT hr;
	//res / shader / SHOULD BE THIS UPON RLEASE
	//
	//
	hr = D3DReadFileToBlob(vertexFileName, &pVertexBlob);
	DisplayError(hr, L"Vertex shader failed to load");
	hr = D3DReadFileToBlob(pixelFileName, &pPixelBlob);
	DisplayError(hr, L"Pixel shader failed to load");
	//Create the Shader Objects
	hr = pGfx->getDevice()->CreateVertexShader(pVertexBlob->GetBufferPointer(),
		pVertexBlob->GetBufferSize(), nullptr, &pVertexShader);
	DisplayError(hr, L"Vertex shader failed to create");
	hr = pGfx->getDevice()->CreatePixelShader(pPixelBlob->GetBufferPointer(),
		pPixelBlob->GetBufferSize(), nullptr, &pPixelShader);
	DisplayError(hr, L"Pixel shader failed to create");

	//VERTEX BUFFER STUFF
	vertexCount = vertices.size();
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices.data();

	hr = pGfx->getDevice()->CreateBuffer(&bd, &sd, &pVertexBuffer);
	DisplayError(hr, L"Create Buffer failed");

	//INDEX BUFFER STUFF
	indexCount = indices.size();
	D3D11_BUFFER_DESC ibd = {};
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(UINT) * indexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices.data();
	hr = pGfx->getDevice()->CreateBuffer(&ibd, &isd, &pIndexBuffer);
	DisplayError(hr, L"Creation of Index Buffer Failed");

	hr = pGfx->getDevice()->CreateInputLayout(layout, std::size(layout), pVertexBlob->GetBufferPointer(),
		pVertexBlob->GetBufferSize(), &pVertexLayout);
	DisplayError(hr, L"Create Input layout failed");

	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.CPUAccessFlags = 0;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(ConstantBuffer);

	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	hr = pGfx->getDevice()->CreateBuffer(&cbd, &csd, &pConstantBuffer);
	DisplayError(hr, L"Create Buffer failed");

	hr = CreateDDSTextureFromFile(pGfx->getDevice(), texFilename, nullptr, &pTexture);
	DisplayError(hr, L"Texture failed to load!");

	D3D11_SAMPLER_DESC tsd = {};
	tsd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	tsd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	tsd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	tsd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	tsd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	tsd.MinLOD = 0;
	tsd.MaxLOD = D3D11_FLOAT32_MAX;

	hr = pGfx->getDevice()->CreateSamplerState(&tsd, &pTexSamplerState);
	DisplayError(hr, L"Create sampler state failed.");

	if (pVertexBlob) pVertexBlob->Release();
	if (pPixelBlob) pPixelBlob->Release();
}

void Model::update()
{
}

void Model::draw()
{
	//pGfx->getContext()->UpdateSubresource(pConstantBuffer, 0, NULL, &cb, 0, 0);
	//pGfx->getContext()->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	pGfx->getContext()->PSSetShaderResources(0, 1, &pTexture);
	pGfx->getContext()->PSSetSamplers(0, 1, &pTexSamplerState);

	if (pIndexBuffer) 
		pGfx->getContext()->DrawIndexed(indexCount, 0u, 0u);
	else
		pGfx->getContext()->Draw(vertexCount, 0u);
}

void Model::bind()
{
	pGfx->getContext()->VSSetShader(pVertexShader, nullptr, 0u);
	pGfx->getContext()->PSSetShader(pPixelShader, nullptr, 0u);
	pGfx->getContext()->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, &offset);
	if (pIndexBuffer) pGfx->getContext()->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0u);
	pGfx->getContext()->IASetInputLayout(pVertexLayout);
	pGfx->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Model::unbind()
{
	pGfx->getContext()->VSSetShader(nullptr, nullptr, 0u);
	pGfx->getContext()->PSSetShader(nullptr, nullptr, 0u);
	pGfx->getContext()->IASetVertexBuffers(0u, 0u, nullptr, nullptr, nullptr);
}
