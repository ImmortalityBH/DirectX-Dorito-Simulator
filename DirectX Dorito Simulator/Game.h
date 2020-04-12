#pragma once

#include "Window.h"
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
	std::unique_ptr<Triangle> tri;
};

