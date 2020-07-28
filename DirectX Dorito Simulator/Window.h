#pragma once

#include "Win.h"
#include "Graphics.h"
#include <memory>

#include "Keyboard.h"
//#include <Mouse.h>
#include "Gamepad.h"
#include <Audio.h>
#include "Mouse.h"

class Window
{
public:
	Window() = default;
	Window(int width, int height, LPCWSTR title);
	~Window();

	bool init(HINSTANCE hInstance);
	void setTitle(LPCWSTR text);
	Graphics& getGraphics();

	int getWidth() const { return width; }
	int getHeight() const { return height; }

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg,
		WPARAM wParam, LPARAM lParam);
public:
	Mouse mouse;
	Keyboard kbd;
	Gamepad gamepad;
	std::unique_ptr<DirectX::AudioEngine> aud;
private:
	Graphics gfx;
	int width, height;
	LPCTSTR WndClassName = L"window";
	LPCWSTR title;
	HWND hWnd;
};

