#pragma once

#include "Window.h"
#include "Timer.h"
#include "Model.h"
#include "Camera.h"

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
	std::unique_ptr<Model> floorModel;
	std::unique_ptr<Model> dogModel;
	Camera camera;
	int randomNum;
	int taskNum;
	bool didRequest = true;

	float oldTimeSinceStart = 0;
};

