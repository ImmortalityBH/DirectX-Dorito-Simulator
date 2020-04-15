#pragma once

#include "Win.h"

inline void DisplayError(HRESULT hr, LPCWSTR what)
{
	if (FAILED(hr))
	{
		MessageBox(nullptr, what, L"Error", MB_OK | MB_ICONERROR);
	}
}