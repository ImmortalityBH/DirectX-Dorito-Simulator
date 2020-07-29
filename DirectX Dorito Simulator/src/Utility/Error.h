#pragma once

#include "Win.h"
#include <string>
#include <exception>
#include <comdef.h>

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

static std::wstring string_to_wide(const std::string& str)
{
	std::wstring wideStr(str.cbegin(), str.cend());
	return wideStr;
}

class BrianException
{
public:
	BrianException(HRESULT hr, const std::string& msg, const std::string& file, const std::string& function, std::size_t line)
	{
		_com_error err(hr);
		whatStr = string_to_wide(msg) + L"\n";
		whatStr += err.ErrorMessage();
		whatStr += L"\nFile: " + string_to_wide(file);
		whatStr += L"\nFunction: " + string_to_wide(function);
		whatStr += L"\nLine: " + string_to_wide(std::to_string(line));
	}
	const LPCWSTR what() const { return whatStr.c_str(); }
private:
	std::wstring whatStr;
};