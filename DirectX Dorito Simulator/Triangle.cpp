#include "Triangle.h"

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
	if (pVertexShader != nullptr)
	{
		pVertexShader->Release();
	}
	if (pPixelShader != nullptr)
	{
		pPixelShader->Release();
	}
}

void Triangle::Create()
{
    HRESULT hr;

    ID3DBlob* pVertexBlob;
    ID3DBlob* pPixelBlob;
    ID3D11InputLayout* vertexLayout;

    hr = D3DReadFileToBlob(L"VertexShader.cso", &pVertexBlob);
    hr = D3DReadFileToBlob(L"PixelShader.cso", &pPixelBlob);
    //Create the Shader Objects
    hr = pGfx->getDevice()->CreateVertexShader(pVertexBlob->GetBufferPointer(),
        pVertexBlob->GetBufferSize(), nullptr, &pVertexShader);
    hr = pGfx->getDevice()->CreatePixelShader(pPixelBlob->GetBufferPointer(),
        pPixelBlob->GetBufferSize(), nullptr, &pPixelShader);

    pGfx->getContext()->VSSetShader(pVertexShader, nullptr, 0u);
    pGfx->getContext()->PSSetShader(pPixelShader, nullptr, 0u);

    Vertex vertices[] =
    {
        Vertex(0.0f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f),
        Vertex(0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f),
        Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f),
    };

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = ARRAYSIZE(layout);

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(Vertex) * 3;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;

    hr = pGfx->getDevice()->CreateBuffer(&bd, &sd, &pVertexBuffer);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    pGfx->getContext()->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, &offset);

    //Create the Input Layout
    hr = pGfx->getDevice()->CreateInputLayout(layout, numElements, pVertexBlob->GetBufferPointer(),
        pVertexBlob->GetBufferSize(), &vertexLayout);

    //Set the Input Layout
    pGfx->getContext()->IASetInputLayout(vertexLayout);
    vertexLayout->Release();
    pVertexBlob->Release();
    pPixelBlob->Release();
    //Set Primitive Topology
    pGfx->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void Triangle::Draw()
{
	pGfx->getContext()->Draw(3u, 0u);
}
