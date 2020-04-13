#include "Triangle.h"

using namespace DirectX;

Triangle::Triangle(Graphics &gfx)
{
	pGfx = &gfx;
}

Triangle::~Triangle()
{
	if (pVertexBuffer != nullptr)
	{
		pVertexBuffer->Release();
	}
    if (pConstantBuffer != nullptr)
    {
        pConstantBuffer->Release();
    }
	if (pVertexShader != nullptr)
	{
		pVertexShader->Release();
	}
	if (pPixelShader != nullptr)
	{
		pPixelShader->Release();
	}
    if (pVertexLayout != nullptr)
    {
        pVertexLayout->Release();
    }
}

void Triangle::Create()
{
    HRESULT hr;

    Vertex vertices[] =
    {
        Vertex(0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f),
        Vertex(0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f),
        Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f),
    };
    
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(vertices);
    bd.StructureByteStride = sizeof(Vertex);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;

    hr = pGfx->getDevice()->CreateBuffer(&bd, &sd, &pVertexBuffer);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    pGfx->getContext()->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, &offset);
    
    
    D3D11_BUFFER_DESC cbd;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbd.MiscFlags = 0u;
    cbd.ByteWidth = sizeof(cb);
    cbd.StructureByteStride = 0u;
    D3D11_SUBRESOURCE_DATA csd = {};
    csd.pSysMem = &cb;
    pGfx->getDevice()->CreateBuffer(&cbd, &csd, &pConstantBuffer);
    pGfx->getContext()->VSSetConstantBuffers(0u, 1u, &pConstantBuffer);
    
    ID3DBlob* pVertexBlob;
    ID3DBlob* pPixelBlob;

    hr = D3DReadFileToBlob(L"VertexShader.cso", &pVertexBlob);
    hr = D3DReadFileToBlob(L"PixelShader.cso", &pPixelBlob);
    //Create the Shader Objects
    hr = pGfx->getDevice()->CreateVertexShader(pVertexBlob->GetBufferPointer(),
        pVertexBlob->GetBufferSize(), nullptr, &pVertexShader);
    hr = pGfx->getDevice()->CreatePixelShader(pPixelBlob->GetBufferPointer(),
        pPixelBlob->GetBufferSize(), nullptr, &pPixelShader);

    pGfx->getContext()->VSSetShader(pVertexShader, nullptr, 0u);
    pGfx->getContext()->PSSetShader(pPixelShader, nullptr, 0u);


    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = ARRAYSIZE(layout);

    //Create the Input Layout
    hr = pGfx->getDevice()->CreateInputLayout(layout, numElements, pVertexBlob->GetBufferPointer(),
        pVertexBlob->GetBufferSize(), &pVertexLayout);

    //Set the Input Layout
    pGfx->getContext()->IASetInputLayout(pVertexLayout);

    //Set Primitive Topology
    pGfx->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    pVertexBlob->Release();
    pPixelBlob->Release();

    transform = XMMatrixIdentity();
    transform = scale * rotation * translation;
    cb.transform = XMMatrixTranspose(transform);
    pGfx->getContext()->UpdateSubresource(pConstantBuffer, 0u, nullptr, &cb, 0u, 0u);
    pGfx->getContext()->VSSetConstantBuffers(0u, 1u, &pConstantBuffer);
}

void Triangle::Update()
{
    //cb.transform = XMMatrixIdentity();
    //cb.transform = XMMatrixTranspose(scale * rotation);
   // pGfx->getContext()->UpdateSubresource(pConstantBuffer, 0u, nullptr, &cb, 0u, 0u);
    //pGfx->getContext()->VSSetConstantBuffers(0u, 1u, &pConstantBuffer);
}

void Triangle::Draw()
{
   
	pGfx->getContext()->Draw(3u, 0u);
}

void Triangle::Scale(float x, float y, float z)
{
    scale = XMMatrixScaling(x, y, z);
}
void Triangle::Rotate(float angle)
{
    rotation = XMMatrixRotationZ(angle);
}

void Triangle::Move(float x, float y, float z)
{
    transform = XMMatrixTranslation(x, y, z);
}
