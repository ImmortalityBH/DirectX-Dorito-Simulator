#pragma once

#include "Window.h"
#include "Timer.h"
#include "Triangle.h"

class Game
{
public:
	Game();

	int run(HINSTANCE hInstance);

	void UpdateScene();
	void DrawScene();

private:
	Window wnd;
	Timer timer;
	std::unique_ptr<Triangle> tri;
};

