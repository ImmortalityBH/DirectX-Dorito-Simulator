#pragma once

#include <DirectXMath.h>

#define ReleaseCOM(x) if (x) { x->Release(); x = nullptr; }

#define DeletePtr(x) if (x) { delete x; x = nullptr; }



namespace Color
{
	XMGLOBALCONST DirectX::XMVECTORF32 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Silver = { 0.75f, 0.75f, 0.75f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };
}

class StringHelper
{
public:
};