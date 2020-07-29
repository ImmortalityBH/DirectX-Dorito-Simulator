#pragma once

#include "Win.h"
#include "Graphics/Graphics.h"
#include <memory>

#include "Input/Mouse.h"
#include "Input/Keyboard.h"
#include "Input/Gamepad.h"
#include "Audio/Audio.h"

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

