#pragma once

#include "Window.h"
#include "Timer.h"
#include "Model.h"

class Game
{
public:
	Game();
	~Game();
	int run(HINSTANCE hInstance);

	void Init();
	void UpdateScene();
	void DrawScene();

private:
	Window wnd; 
	Timer timer;
private:
	std::unique_ptr<Model> model;
	int randomNum;
	int taskNum;
	bool didRequest = true;

	float oldTimeSinceStart = 0;
};

