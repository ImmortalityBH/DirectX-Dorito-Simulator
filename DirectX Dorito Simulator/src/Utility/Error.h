#pragma once

#include "Win.h"
#include <string>
#include <exception>
#include <comdef.h>

class BrianException;
#define THROW_IF_FAILED(hr, msg) if (FAILED(hr)) throw BrianException(hr, msg, __FILE__, __FUNCTION__, __LINE__)
#define THROW_NORMAL(msg) throw BrianException(msg, __FILE__, __FUNCTION__, __LINE__)

inline std::wstring string_to_wstring(const std::string & str)
{
	std::wstring wideStr(str.cbegin(), str.cend());
	return wideStr;
}

inline std::string wstring_to_string(const std::wstring& str)
{
	std::string normStr(str.cbegin(), str.cend());
	return normStr;
}

class BrianException
{
public:
	BrianException(HRESULT hr, const std::string& msg, const std::string& file, 
		const std::string& function, std::size_t line)
	{
		_com_error err(hr);
		whatStr = string_to_wstring(msg) + L"\n";
		whatStr += err.ErrorMessage();
		whatStr += L"\nFile: " + string_to_wstring(file);
		whatStr += L"\nFunction: " + string_to_wstring(function);
		whatStr += L"\nLine: " + string_to_wstring(std::to_string(line));
	}
	BrianException(const std::string& msg, const std::string& file,
		const std::string& function, std::size_t line)
	{
		whatStr = string_to_wstring(msg) + L"\n";
		whatStr += L"\nFile: " + string_to_wstring(file);
		whatStr += L"\nFunction: " + string_to_wstring(function);
		whatStr += L"\nLine: " + string_to_wstring(std::to_string(line));
	}
	const LPCWSTR what() const { return whatStr.c_str(); }
private:
	std::wstring whatStr;
};

class ErrorLogger
{
public:
	static void Log(HRESULT hr, std::wstring what)
	{
		if (FAILED(hr))
		{
			MessageBoxW(nullptr, what.c_str(), L"Error", MB_ICONERROR);
		}
	}
	static void Log(std::wstring what)
	{
		MessageBoxW(nullptr, what.c_str(), L"User Defined Error", MB_ICONERROR);
	}
	static void Log(std::wstring what, std::wstring title)
	{
		MessageBoxW(nullptr, what.c_str(), title.c_str(), MB_OK);
	}
	static void Log(BrianException& e)
	{
		MessageBoxW(nullptr, e.what(), L"Error", MB_ICONERROR);
	}
};
