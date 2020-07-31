#pragma once

#include "Window/Window.h"
#include "Utility/Timer.h"
#include "Graphics/Model.h"
#include "Camera.h"

#include <memory>
#include "Graphics/Texture.h"
#include "Graphics/Model.h"

class Game
{
public:
	Game();
	~Game();
	int run(HINSTANCE hInstance);

	void init();
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

	Texture dorTex, florTex, dogTex, bagTex, mtnTex, catTex;
	//commented out for now
	//std::unique_ptr<DirectX::SoundEffect> intro;
	//std::unique_ptr<DirectX::SoundEffect> outro;
	//std::unique_ptr<DirectX::SoundEffect> effects[13];
	char title[12] = { 0 };
	Camera camera;
	int randomNum;
	int taskNum;
	bool didRequest = true;
};

