#pragma once

#include "Win.h"
#include <memory>
#include <string>

#include "Graphics/Graphics.h"
#include "Input/Mouse.h"
#include "Input/Keyboard.h"
#include "Input/Gamepad.h"
#include <Audio.h>
//#include "Audio/Audio.h"

class Window
{
public:
	Window();
	~Window();

	bool init(HINSTANCE hInstance);
	void setTitle(LPCWSTR text);
	Graphics& getGraphics();

	UINT getWidth() const { return width; }
	UINT getHeight() const { return height; }
private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg,
		WPARAM wParam, LPARAM lParam);
public:
	Mouse mouse;
	Keyboard kbd;
	Gamepad gamepad;
	//AudioEngine audioEngine;
	std::unique_ptr<DirectX::AudioEngine> aud;
private:
	Graphics gfx;
	UINT width, height;
	LPCTSTR WndClassName = L"window";
	std::wstring title;
	HWND hWnd;
};

static bool load_config(const char* filePath, bool& isFullscreen, UINT& width,
	UINT& height, bool& isVysnc);