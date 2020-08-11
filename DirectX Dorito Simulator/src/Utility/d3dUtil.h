#pragma once

#include <DirectXMath.h>
#include <string>

//#define ReleaseCOM(x) if (x != nullptr) { x->Release(); x = nullptr; }
template<class T>
inline void ReleaseCOM(T* pInterface)
{
	if (pInterface != nullptr)
	{
		pInterface->Release();
		pInterface = nullptr;
	}
}

template<class T>
inline void ReleaseCOM(T** ppInterface)
{
	if (*ppInterface != nullptr)
	{
		(*ppInterface)->Release();
		(*ppInterface) = nullptr;
	}
}

template<class T>
inline void DeletePtr(T* pPointer)
{
	if (pPointer != nullptr)
	{
		delete pPointer;
		pPointer = nullptr;
	}
}
//#define DeletePtr(x) if (x) { delete x; x = nullptr; }

#define CONFIG_PATH "config.ini"

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

inline std::wstring string_to_wstring(const std::string& str)
{
	std::wstring wideStr(str.cbegin(), str.cend());
	return wideStr;
}

inline std::string wstring_to_string(const std::wstring& str)
{
	std::string normStr(str.cbegin(), str.cend());
	return normStr;
}