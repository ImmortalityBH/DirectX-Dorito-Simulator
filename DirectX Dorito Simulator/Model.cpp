#include "Model.h"
#include "Error.h"

Model::Model(Graphics& gfx, LPCWSTR texFilename)
	: pGfx(&gfx), texFilename(texFilename)
{
}

Model::~Model()
{
	if (pVertexBuffer != nullptr) {
		pVertexBuffer->Release();
	}
	if (pConstantBuffer != nullptr) {
		pConstantBuffer->Release();
	}
	if (pTexture != nullptr) {
		pTexture->Release();
	}
	if (pTexSamplerState != nullptr) {
		pTexSamplerState->Release();
	}
	if (pVertexLayout != nullptr) {
		pVertexLayout->Release();
	}
	if (pVertexShader != nullptr) {
		pVertexShader->Release();
	}
	if (pPixelShader != nullptr) {
		pPixelShader->Release();
	}
}

void Model::create()
{
}

void Model::update()
{
}

void Model::draw()
{
}

void Model::bindShaders()
{
	pGfx->getContext()->VSSetShader(pVertexShader, nullptr, 0u);
	pGfx->getContext()->PSSetShader(pPixelShader, nullptr, 0u);
}

void Model::unbindShaders()
{
	pGfx->getContext()->VSSetShader(nullptr, nullptr, 0u);
	pGfx->getContext()->PSSetShader(nullptr, nullptr, 0u);
}
