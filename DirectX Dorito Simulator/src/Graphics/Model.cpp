#include "Model.h"
#include "Utility/Error.h"
#include "Utility/d3dUtil.h"

using namespace DirectX;

Model::Model(Graphics& gfx)
	: pGfx(&gfx)
{
}

Model::~Model()
{
}

void Model::create(std::vector<Vertex>& vertices)
{
	HRESULT hr = S_OK;
	
	hr = vertexBuffer.init(pGfx->getDevice(), vertices.data(), vertices.size());
	ErrorLogger::Log(hr, L"Vertex buffer initialization failed");

	hr = constantBuffer.init(pGfx->getDevice(), pGfx->getContext());
	ErrorLogger::Log(hr, L"const buffer init failed");
	/*vertexCount = vertices.size();
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices.data();

	hr = pGfx->getDevice()->CreateBuffer(&bd, &sd, &pVertexBuffer);
	ErrorLogger::Log(hr, L"Create Buffer failed");

	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//cbd.Usage = D3D11_USAGE_DEFAULT;//D3D11_USAGE_DYNAMIC
	//cbd.CPUAccessFlags = 0u;//D3D11_CPU_ACCESS_WRITE
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(CB_WVP);
	cbd.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	pGfx->getDevice()->CreateBuffer(&cbd, &csd, &pConstantBuffer);
	ErrorLogger::Log(hr, L"Create Buffer failed");*/
}

void Model::create(std::vector<Vertex>& vertices, std::vector<DWORD>& indices)
{
	//////////////////////////////////////////////////
	this->isIndexed = true;
	//////////////////////////////////////////////////

	HRESULT hr = S_OK;

	hr = vertexBuffer.init(pGfx->getDevice(), vertices.data(), vertices.size());
	ErrorLogger::Log(hr, L"Vertex buffer init failed");

	hr = indexBuffer.init(pGfx->getDevice(), indices.data(), indices.size());
	ErrorLogger::Log(hr, L"Index buffer init failed");

	hr = constantBuffer.init(pGfx->getDevice(), pGfx->getContext());
	ErrorLogger::Log(hr, L"const buffer init failed");
	/*//VERTEX BUFFER STUFF
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
	ErrorLogger::Log(hr, L"Create Buffer failed");

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
	ErrorLogger::Log(hr, L"Creation of Index Buffer Failed");

	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(CB_WVP);
	cbd.StructureByteStride = 0u;

	//D3D11_SUBRESOURCE_DATA csd = {};
	//csd.pSysMem = &cb;
	hr = pGfx->getDevice()->CreateBuffer(&cbd, nullptr, &pConstantBuffer);
	ErrorLogger::Log(hr, L"Create Buffer failed");*/
}

void Model::resetMatrix()
{
	sca = XMMatrixIdentity();
	rotation = XMMatrixIdentity();
	translation = XMMatrixIdentity();
}

void Model::update(float dt, Camera& camera)
{
	//move
	translation = XMMatrixTranslation(transform.position.x, 
		transform.position.y, transform.position.z);
	//scale
	sca = XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);
	//rotation
	//XMVECTOR quat = XMQuaternionRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);
	//rotation = XMMatrixRotationQuaternion(quat);

	world = sca * rotation * translation;

	WVP = world * camera.getView() * camera.getProjection();
	constantBuffer.data.WVP = XMMatrixTranspose(WVP);

}

void Model::move(float x, float y, float z)
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

void Model::rotate(float x, float y, float z, float Angle)
{
	XMVECTOR vec = XMVectorSet(x, y, z, 1.0f);
	rotation = XMMatrixRotationAxis(vec, XMConvertToRadians(Angle));
}

void Model::setRotation(float x, float y, float z, float Angle)
{
	XMVECTOR vec = XMVectorSet(x, y, z, 1.0f);
	rotation = XMMatrixRotationAxis(vec, XMConvertToRadians(Angle));
	//transform.rotation.x = x;
	//transform.rotation.y = y;
	//transform.rotation.z = z;
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
	/*pGfx->getContext()->UpdateSubresource(pConstantBuffer, 0, NULL, &cb, 0, 0);
	pGfx->getContext()->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	D3D11_MAPPED_SUBRESOURCE mappedResource = {};

	pGfx->getContext()->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	CopyMemory(mappedResource.pData, &cb, sizeof(CB_WVP));

	pGfx->getContext()->Unmap(pConstantBuffer, 0);*/

	pGfx->getContext()->VSSetConstantBuffers(0, 1, constantBuffer.getBuffer());
	constantBuffer.update();

	if (isIndexed) 
		pGfx->getContext()->DrawIndexed(indexBuffer.getIndexCount(), 0u, 0u);
	else
		pGfx->getContext()->Draw(vertexBuffer.getVertexCount(), 0u);
}

void Model::bind(VertexShader& vs, PixelShader& ps, Texture& tex)
{
	pGfx->getContext()->VSSetShader(vs.getVertexShader(), nullptr, 0u);
	pGfx->getContext()->PSSetShader(ps.getPixelShader(), nullptr, 0u);
	pGfx->getContext()->PSSetShaderResources(0, 1, tex.getTexture());
	pGfx->getContext()->PSSetSamplers(0, 1, tex.getSamplerState());
	pGfx->getContext()->IASetVertexBuffers(0u, 1u, vertexBuffer.getBuffer(), vertexBuffer.getStridePtr(), &offset);
	if (isIndexed) pGfx->getContext()->IASetIndexBuffer(indexBuffer.getBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	pGfx->getContext()->IASetInputLayout(vs.getInputLayout());
	pGfx->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Model::unbind()
{
	pGfx->getContext()->VSSetShader(nullptr, nullptr, 0u);
	pGfx->getContext()->PSSetShader(nullptr, nullptr, 0u);
	pGfx->getContext()->IASetVertexBuffers(0u, 0u, nullptr, nullptr, nullptr);
	if (isIndexed) pGfx->getContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0u);
}
