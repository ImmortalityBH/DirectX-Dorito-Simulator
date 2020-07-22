#pragma once

#include "Window.h"
#include "Timer.h"
#include "Model.h"
#include "Camera.h"
#include <Audio.h>
#include <memory>

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
	Timer elapsedTimer;
private:
	std::unique_ptr<Model> dorito;
	std::unique_ptr<Model> floorModel;
	std::unique_ptr<Model> dogModel;
	std::unique_ptr<Model> bagModel;
	std::unique_ptr<Model> mtnDewModel;

	std::unique_ptr<DirectX::SoundEffect> intro;
	std::unique_ptr<DirectX::SoundEffect> outro;
	std::unique_ptr<DirectX::SoundEffect> effects[13];

	Camera camera;
	int randomNum;
	int taskNum;
	bool didRequest = true;

	float oldTimeSinceStart = 0;
	bool isWireframe = false;
};

