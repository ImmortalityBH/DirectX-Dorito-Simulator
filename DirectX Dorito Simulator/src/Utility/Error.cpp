#include "Utility/Error.h"
#include "Utility/d3dUtil.h"
#include <sstream>

HrException::HrException(HRESULT hr, const std::string& msg, const std::string& file,
	const std::string& function, std::size_t line) noexcept
	: file(file), function(function), line(line)
{
	_com_error err(hr);
	std::ostringstream oss;

	oss << msg << "\n" << wstring_to_string(err.ErrorMessage()) << "\nFile: " << file <<
		"\nFunction: " << function << "\nLine: " << std::to_string(line);
	whatStr = oss.str();
}

HrException::HrException(const std::string& msg, const std::string& file,
	const std::string& function, std::size_t line) noexcept
	: file(file), function(function), line(line)
{
	std::ostringstream oss;

	oss << msg << "\n" << "\nFile: " << file <<
		"\nFunction: " << function << "\nLine: " << std::to_string(line);
	whatStr = oss.str();
}

const char* HrException::getType() const noexcept
{
	return "Base Exception";
}

inline std::size_t HrException::getLine() const noexcept
{
	return line;
}

inline const std::string& HrException::getFunction() const noexcept
{
	return function;
}

inline const std::string& HrException::getFile() const noexcept
{
	return file;
}

void ErrorLogger::Log(HRESULT hr, std::wstring what)
{
	if (FAILED(hr))
	{
		MessageBoxW(nullptr, what.c_str(), L"Error", MB_ICONERROR);
	}
}
 
void ErrorLogger::Log(std::wstring what)
{
	MessageBoxW(nullptr, what.c_str(), L"User Defined Error", MB_ICONERROR);
}

void ErrorLogger::Log(std::wstring what, std::wstring title)
{
	MessageBoxW(nullptr, what.c_str(), title.c_str(), MB_OK);
}

 void ErrorLogger::Log(HrException& e)
{
	std::wstring wstr = string_to_wstring(e.what());
	MessageBoxW(nullptr, wstr.c_str(), L"Error", MB_ICONERROR);
}
