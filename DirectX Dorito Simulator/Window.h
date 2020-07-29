#pragma once

#include "Win.h"
#include "Graphics.h"
#include <memory>

#include "Mouse.h"
#include "Keyboard.h"
#include "Gamepad.h"
#include "Audio.h"

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
	AudioEngine audioEngine;
private:
	Graphics gfx;
	int width, height;
	LPCTSTR WndClassName = L"window";
	LPCWSTR title;
	HWND hWnd;
};

