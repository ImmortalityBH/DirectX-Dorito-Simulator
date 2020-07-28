#pragma once

#include "Win.h"
#include <string>

inline void DisplayError(HRESULT hr, std::wstring what)
{
	if (FAILED(hr))
	{
		MessageBoxW(nullptr, what.c_str(), L"Error", MB_OK | MB_ICONERROR);
	}
}

inline void DisplayError(std::wstring what)
{
	MessageBoxW(nullptr, what.c_str(), L"User Defined Error", MB_OK | MB_ICONERROR);
}

inline void DisplayMB(std::wstring what, std::wstring title)
{
	MessageBoxW(nullptr, what.c_str(), title.c_str(), MB_OK);
}