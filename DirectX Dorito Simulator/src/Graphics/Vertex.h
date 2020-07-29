#pragma once

#include <DirectXMath.h>

struct Vertex2D
{
	Vertex2D(float x, float y, float u,
		float v) : pos(x, y, 0.0f), uv(u, v)
	{
	}
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

struct VertexColor2D
{
	VertexColor2D(float x, float y, float r, float g, float b, float a = 1.0f)
		: pos(x, y, 0.0f), color{ r, g, b, a }
	{
	}
	VertexColor2D(float x, float y, float r, DirectX::XMVECTORF32 col)
		: pos(x, y, 0.0f), color{ col }
	{
	}
	DirectX::XMFLOAT3 pos;
	DirectX::XMVECTORF32 color;
};

struct Vertex
{
	Vertex(float x, float y, float z,
		float u, float v)
		: pos(x, y, z), uv(u, v)
	{
	}
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

struct VertexColor
{
	VertexColor(float x, float y, float z,
		float r, float g, float b, float a = 1.0f)
		: pos(x, y, z), color{ r, g, b, a }
	{
	}
	VertexColor(float x, float y, float z,
		DirectX::XMVECTORF32 col)
		: pos(x, y, z), color{ col }
	{
	}
	DirectX::XMFLOAT3 pos;
	DirectX::XMVECTORF32 color;
};
