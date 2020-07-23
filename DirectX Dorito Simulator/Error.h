#pragma once

#include "Win.h"

inline void DisplayError(HRESULT hr, LPCWSTR what)
{
	if (FAILED(hr))
	{
		MessageBox(nullptr, what, L"Error", MB_OK | MB_ICONERROR);
	}
}

inline void DisplayError(LPCWSTR what) 
{
	MessageBox(nullptr, what, L"User Defined Error", MB_OK | MB_ICONERROR);
}

inline void DisplayMB(LPCWSTR what, LPCWSTR title)
{
	MessageBox(nullptr, what, title, MB_OK);
}