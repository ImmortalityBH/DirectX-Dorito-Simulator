#include "Model.h"
#include "Utility/Error.h"
#include "Utility/d3dUtil.h"

using namespace DirectX;

Model::Model()
{
}

Model::~Model()
{
}

void Model::create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, std::vector<Vertex>& vertices)
{
	//////////////////////////////////////////////////
	this->pContext = pContext;
	//////////////////////////////////////////////////

	HRESULT hr = S_OK;
	
	hr = vertexBuffer.init(pDevice, vertices.data(), vertices.size());
	ErrorLogger::Log(hr, L"Vertex buffer initialization failed");

	hr = constantBuffer.init(pDevice, pContext);
	ErrorLogger::Log(hr, L"const buffer init failed");
}

void Model::create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, std::vector<Vertex>& vertices, std::vector<DWORD>& indices)
{
	//////////////////////////////////////////////////
	this->isIndexed = true;
	this->pContext = pContext;
	//////////////////////////////////////////////////

	HRESULT hr = S_OK;

	hr = vertexBuffer.init(pDevice, vertices.data(), vertices.size());
	ErrorLogger::Log(hr, L"Vertex buffer init failed");

	hr = indexBuffer.init(pDevice, indices.data(), indices.size());
	ErrorLogger::Log(hr, L"Index buffer init failed");

	hr = constantBuffer.init(pDevice, pContext);
	ErrorLogger::Log(hr, L"const buffer init failed");
}

bool Model::isColliding(Model& model)
{
	const float boxSize = 0.5f;
	XMFLOAT4 min = { model.transform.position.x - boxSize, model.transform.position.y - boxSize, model.transform.position.z - boxSize, 0.0f };
	XMFLOAT4 max = { model.transform.position.x + boxSize, model.transform.position.y + boxSize, model.transform.position.z + boxSize, 0.0f };

	if ((transform.position.x >= min.x && transform.position.x <= max.x) &&
		(transform.position.y >= min.y && transform.position.y <= max.y) &&
		(transform.position.y >= min.y && transform.position.z <= max.z))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Model::resetMatrix()
{
	translation = XMMatrixIdentity();
	rotation = XMMatrixIdentity();
	sca = XMMatrixIdentity();
}

void Model::updateMatrix(Camera& camera)
{
	//scale
	/*XMMATRIX sca = XMMatrixScaling(transform.scale.x, transform.scale.y,
		transform.scale.z);
	//rotate
	//XMMATRIX rotation = XMMatrixRotationRollPitchYaw(transform.rotation.x,
		//transform.rotation.y, transform.rotation.z);
	//translate
	XMMATRIX translation = XMMatrixTranslation(transform.position.x,
		transform.position.y, transform.position.z);*/

	sca = XMMatrixScaling(transform.scale.x, transform.scale.y,
		transform.scale.z);
	translation = XMMatrixTranslation(transform.position.x,
		transform.position.y, transform.position.z);

	world = sca * rotation * translation;

	WVP = world * camera.getView() * camera.getProjection();
	constantBuffer.data.WVP = XMMatrixTranspose(WVP);
}

void Model::adjustPos(float x, float y, float z)
{
	transform.position.x += x;
	transform.position.y += y;
	transform.position.z += z;
	transform.position.w = 1.0f;
}

void Model::setPos(float x, float y, float z)
{
	transform.position.x = x;
	transform.position.y = y;
	transform.position.z = z;
	transform.position.w = 1.0f;
}

void Model::adjustRot(float x, float y, float z, float angle)
{
	/*transform.rotation.x += x;
	transform.rotation.y += y;
	transform.rotation.z += z;
	transform.rotation.w += 1.0f;*/
	XMVECTOR vect = XMVectorSet(x, y, z, 1.0f);
	rotation = XMMatrixRotationAxis(vect, XMConvertToRadians(angle));
}

void Model::setRot(float x, float y, float z)
{
	/*transform.rotation.x = x;
	transform.rotation.y = y;
	transform.rotation.z = z;
	transform.rotation.w = 1.0f;*/
}

void Model::scale(float x, float y, float z)
{
	transform.scale.x += x;
	transform.scale.y += y;
	transform.scale.z += z;
	transform.scale.w = 1.0f;
}

void Model::setScale(float x, float y, float z)
{
	transform.scale.x = x;
	transform.scale.y = y;
	transform.scale.z = z;
	transform.scale.w = 1.0f;
}

void Model::draw()
{
	//pGfx->getContext()->UpdateSubresource(pConstantBuffer, 0, NULL, &cb, 0, 0);

	pContext->VSSetConstantBuffers(0, 1, constantBuffer.getBuffer());
	constantBuffer.update();

	if (isIndexed) 
		pContext->DrawIndexed(indexBuffer.getIndexCount(), 0u, 0u);
	else
		pContext->Draw(vertexBuffer.getVertexCount(), 0u);
}

void Model::bind(VertexShader& vs, PixelShader& ps, Texture& tex)
{
	const UINT offset = 0;
	pContext->VSSetShader(vs.getVertexShader(), nullptr, 0u);
	pContext->PSSetShader(ps.getPixelShader(), nullptr, 0u);
	pContext->PSSetShaderResources(0, 1, tex.getTexture());
	//pContext->PSSetSamplers(0, 1, pGfx->getSamplerState());
	pContext->IASetVertexBuffers(0u, 1u, vertexBuffer.getBuffer(), vertexBuffer.getStridePtr(), &offset);
	if (isIndexed) pContext->IASetIndexBuffer(indexBuffer.getBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	pContext->IASetInputLayout(vs.getInputLayout());
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Model::unbind()
{
	pContext->VSSetShader(nullptr, nullptr, 0u);
	pContext->PSSetShader(nullptr, nullptr, 0u);
	pContext->IASetVertexBuffers(0u, 0u, nullptr, nullptr, nullptr);
	if (isIndexed) pContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0u);
}
