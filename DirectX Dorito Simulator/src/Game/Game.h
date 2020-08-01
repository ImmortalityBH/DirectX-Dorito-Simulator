#pragma once

#include "Window/Window.h"
#include "Utility/Timer.h"
#include "Graphics/Model.h"
#include "Camera.h"

#include <memory>
#include "Graphics/Texture.h"
#include "Graphics/Model.h"

enum GAME_STATE 
{

};

class Game
{
public:
	Game();
	~Game();

	void init(HINSTANCE hInstance);
	void update();
	void draw();

private:
	Window wnd; 
	Timer timer;
	Timer elapsedTimer;
	Timer fpsTimer;
private:
	std::unique_ptr<Model> dorito;
	std::unique_ptr<Model> floorModel;
	std::unique_ptr<Model> dogModel;
	std::unique_ptr<Model> bagModel;
	std::unique_ptr<Model> mtnDewModel;
	std::unique_ptr<Model> coinModel;

	Texture dorTex, florTex, dogTex, bagTex, mtnTex, catTex, coinTex;
	//commented out for now
	std::unique_ptr<DirectX::SoundEffect> intro;
	std::unique_ptr<DirectX::SoundEffect> coinSound;
	//std::unique_ptr<DirectX::SoundEffect> outro;
	//std::unique_ptr<DirectX::SoundEffect> effects[13];
	char title[12] = { 0 };
	Camera camera;
	int randomNum;
	int taskNum;
	bool doOnce = false;
	bool didRequest = true;
};

