#pragma once

#include "Window/Window.h"
#include "Utility/Timer.h"
#include "Graphics/Model.h"
#include "Camera.h"

#include <memory>
#include "Graphics/Texture.h"
#include "Graphics/Model.h"
#include <map>

enum GAME_STATE 
{

};

class Game
{
public:
	Game();
	~Game();

	bool init();
	int run(HINSTANCE hInst);
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
	std::unique_ptr<Model> sunModel;

	Texture dorTex, florTex, dogTex, bagTex, mtnTex, 
		catTex, coinTex, sunTex;
	//commented out for now
	//std::unique_ptr<DirectX::SoundEffect> intro;
	std::unique_ptr<DirectX::SoundEffect> coinSound;
	//std::unique_ptr<DirectX::SoundEffect> outro;
	//std::unique_ptr<DirectX::SoundEffect> effects[13];
	void setCoinPos();
	static constexpr float minX = -0.8f;
	static constexpr float maxX = 0.8f;
	static constexpr float minY = -0.3f;
	static constexpr float maxY = 1.0f;

	char title[12] = { 0 };
	Camera camera;
	int randomNum;
	int taskNum;
	bool doOnce = false;
	bool didRequest = true;
};

