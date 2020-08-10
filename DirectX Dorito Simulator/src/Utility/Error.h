#pragma once

#include "Win.h"
#include <string>
#include <exception>
#include <comdef.h>

class HrException;
#define THROW_IF_FAILED(hr, msg) if (FAILED(hr)) throw HrException(hr, msg, __FILE__, __FUNCTION__, __LINE__)
#define THROW_NORMAL(msg) throw HrException(msg, __FILE__, __FUNCTION__, __LINE__)

class HrException : public std::exception
{
public:
	HrException(HRESULT hr, const std::string& msg, const std::string& file,
		const std::string& function, std::size_t line) noexcept;
	HrException(const std::string& msg, const std::string& file,
		const std::string& function, std::size_t line) noexcept;
	HrException(const HrException&) = delete;
	HrException& operator=(const HrException&) = delete;
	~HrException() = default;
	const char* what() const noexcept override { return whatStr.c_str(); }
	virtual const char* getType() const noexcept; //not pure 
	std::size_t getLine() const noexcept;
	const std::string& getFunction() const noexcept;
	const std::string& getFile() const noexcept;
private:	
	std::string file;
	std::string function;
	std::size_t line;
protected:
	mutable std::string whatStr;
};

class ErrorLogger
{
public:
	static void Log(HRESULT hr, std::wstring what);
	static void Log(std::wstring what);
	static void Log(std::wstring what, std::wstring title);
	static void Log(HrException& e);
};
