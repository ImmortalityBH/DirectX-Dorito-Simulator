#pragma once

#include "Win.h"
#include "Graphics.h"
#include <memory>

#include <Keyboard.h>
#include <Mouse.h>
#include <GamePad.h>

class Window
{
public:
	Window() = default;
	Window(int width, int height, LPCWSTR title);
	~Window();

	bool init(HINSTANCE hInstance);
	void setTitle(LPCWSTR text);
	Graphics& Gfx();

	int getWidth() const { return width; }
	int getHeight() const { return height; }

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg,
		WPARAM wParam, LPARAM lParam);
public:
	std::unique_ptr<DirectX::Keyboard> kbd;
	std::unique_ptr<DirectX::Mouse> mouse;
	std::unique_ptr<DirectX::GamePad> gamepad;
private:
	std::unique_ptr<Graphics> pGfx;
	int width, height;
	LPCTSTR WndClassName = L"window";
	LPCWSTR title;
	HWND hWnd;
};

