#include "Game/Game.h"
#include <string>
#include <vector>
#include <algorithm>
#include <Utility/Error.h>
#include <thread>

//using namespace DirectX;

Game::Game()
	: wnd(800, 600, L"DirectX Dorito Simulator: Deluxe Edition"),
	  camera(wnd),
	  randomNum(0),
	  taskNum(0)
{
}

Game::~Game()
{
}

int Game::run(HINSTANCE hInstance)
{
	wnd.init(hInstance);
	MSG msg = {};
	init();

	try
	{
		HRESULT hr = E_FAIL;
		THROW_IF_FAILED(hr, "Bruh error");
	}
	catch (BrianException& exception)
	{
		ErrorLogger::Log(exception);
	}


	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			UpdateScene();
			DrawScene();
		}
	}
	return static_cast<int>(msg.wParam);
}

void Game::init()
{

	//intro = std::make_unique<SoundEffect>(wnd.aud.get() , L"res/audio/intro.wav");
	//outro = std::make_unique<SoundEffect>(wnd.aud.get(), L"res/audio/outro.wav");

	auto loadModel = [this]() {
		dorito = std::make_unique<Model>(wnd.getGraphics());
		floorModel = std::make_unique<Model>(wnd.getGraphics());
		dogModel = std::make_unique<Model>(wnd.getGraphics());
		bagModel = std::make_unique<Model>(wnd.getGraphics());
		mtnDewModel = std::make_unique<Model>(wnd.getGraphics());
		//DORITO
		std::vector<Vertex> vertices =
		{
			Vertex(0.0f, 0.5f, 0.0f, 0.5f, 1.0f),
			Vertex(0.5f, -0.5f, 0.0f, 1.0f, 0.0f),
			Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f),
		};
		//FLOOR
		std::vector<Vertex> floorVertices =
		{
			Vertex(-0.5f, 0.5f, 0.0f, 0.0f, 0.0f),
			Vertex(0.5f, -0.5f, 0.0f, 2.0f, 2.0f),
			Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 2.0f),
			Vertex(0.5f, 0.5f, 0.0f, 2.0f, 0.0f),
		};
		std::vector<Vertex> bagVertices =
		{
			Vertex(-0.5f, 0.5f, 0.0f, 0.0f, 0.0f),
			Vertex(0.5f, -0.5f, 0.0f, 1.0f, 1.0f),
			Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 1.0f),
			Vertex(0.5f, 0.5f, 0.0f, 1.0f, 0.0f),
		};
		std::vector<UINT> floorIndices =
		{
			0, 1, 2,
			3, 1, 0,
		};
		/*td::vector<Vertex> boxVertices = {
			{ -1.0f,  1.0f, -1.0f, },
			{ -1.0f, -1.0f, -1.0f, },
			{1.0f, -1.0f, -1.0f,},
			{1.0f, -1.0f, -1.0f,},
			{1.0f,  1.0f, -1.0f,},
			{-1.0f,  1.0f, -1.0f,},

			{-1.0f, -1.0f,  1.0f,},
			{-1.0f, -1.0f, -1.0f,},
			{-1.0f,  1.0f, -1.0f,},
			{-1.0f,  1.0f, -1.0f,},
			{-1.0f,  1.0f,  1.0f,},
			{-1.0f, -1.0f,  1.0f,},

			{1.0f, -1.0f, -1.0f,},
			{1.0f, -1.0f,  1.0f,},
			{ 1.0f,  1.0f,  1.0f,},
			{1.0f,  1.0f,  1.0f,},
			{1.0f,  1.0f, -1.0f,},
			{1.0f, -1.0f, -1.0f,},

			{-1.0f, -1.0f,  1.0f,},
			{-1.0f,  1.0f,  1.0f,},
			{1.0f,  1.0f,  1.0f,},
			{1.0f,  1.0f,  1.0f,},
			{1.0f, -1.0f,  1.0f,},
			{-1.0f, -1.0f,  1.0f,},

			{-1.0f,  1.0f, -1.0f,},
			{ 1.0f,  1.0f, -1.0f,},
			{ 1.0f,  1.0f,  1.0f,},
			{ 1.0f,  1.0f,  1.0f,},
			{-1.0f,  1.0f,  1.0f,},
			{-1.0f,  1.0f, -1.0f,},

			{-1.0f, -1.0f, -1.0f,},
			{-1.0f, -1.0f,  1.0f,},
			{ 1.0f, -1.0f, -1.0f,},
			{ 1.0f, -1.0f, -1.0f,},
			{-1.0f, -1.0f,  1.0f,},
			{ 1.0f, -1.0f,  1.0f},
		};*/
		dorito->create(vertices);
		floorModel->create(floorVertices, floorIndices);
		dogModel->create(bagVertices, floorIndices);
		bagModel->create(bagVertices, floorIndices);
		mtnDewModel->create(bagVertices, floorIndices);

		dorito->setPos(0.0f, 0.35f, 1.5f);
	};
	auto loadTex = [this]() { //no need to check return types handling done in class
		dorTex.load(wnd.getGraphics().getDevice(), L"res/img/dorito.dds");
		florTex.load(wnd.getGraphics().getDevice(), L"res/img/floor.dds");
		dogTex.load(wnd.getGraphics().getDevice(), L"res/img/dog.dds");
		bagTex.load(wnd.getGraphics().getDevice(), L"res/img/bag.dds");
		mtnTex.load(wnd.getGraphics().getDevice(), L"res/img/mtndew.dds");
		catTex.load(wnd.getGraphics().getDevice(), L"res/img/winkcat.dds");
	};                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    

	std::thread modelThread(loadModel);
	std::thread texThread(loadTex);

	modelThread.join();
	texThread.join();

	//Sound sound(wnd.audioEngine, "res/audio/poop.wav");
	wnd.audioEngine.playSound2D("res/audio/poop.wav");
	/*std::vector<std::wstring> fileNames = {
		L"res/audio/amazing.wav",
		L"res/audio/awesome.wav",
		L"res/audio/delicious.wav",
		L"res/audio/good job.wav",
		L"res/audio/incredible.wav",
		L"res/audio/mom.wav",
		L"res/audio/nice one.wav",
		L"res/audio/oh my god.wav",
		L"res/audio/poop.wav",
		L"res/audio/right.wav",
		L"res/audio/un.wav",
		L"res/audio/fire.wav",
		L"res/audio/wow.wav"
	};
	for (std::size_t i = 0; i < fileNames.size(); i++)
	{
		effects[i] = std::make_unique<SoundEffect>(wnd.aud.get(), fileNames[i].c_str());
	}*/
}

void Game::UpdateScene()
{
	float dTime = timer.Peek();
	float eTime = elapsedTimer.Peek();
	timer.Mark();

	//std::wstring title = L"Mouse Pos (" + std::to_wstring(wnd.mouse.getPos().x) + L", " + 
		//std::to_wstring(wnd.mouse.getPos().y) + L", Wheel: " + std::to_wstring(wnd.mouse.getWheelDelta()) + L")";

	if (elapsedTimer.Peek() > 10.0f)
	{
		wnd.setTitle(wnd.kbd.getCharAsString().c_str());
	}

	//std::wstring title =  L"Elapsed Time: " + std::to_wstring(elapsedTimer.Peek()) + L", Delta Time: " + std::to_wstring(dTime);
	//std::wstring forito = L"X: " + std::to_wstring(dorito->transform.position.x) + L", Y:" + std::to_wstring(dorito->transform.position.y);
	//wnd.setTitle(title.c_str());

	camera.update(timer.Peek(), wnd);
	//model->resetMatrix();
	if (wnd.kbd.isKeyPressed(Keyboard::KeyCode::VK_W) || wnd.kbd.isKeyPressed(VK_UP))
	{
		dorito->move(0.0f, dTime * 0.5f, 0.0f);
	} 
	else if (wnd.kbd.isKeyPressed(Keyboard::KeyCode::VK_S) || wnd.kbd.isKeyPressed(VK_DOWN))
	{
		dorito->move(0.0f, -(dTime * 0.5f), 0.0f);
	}
	else if (wnd.kbd.isKeyPressed(Keyboard::KeyCode::VK_A) || wnd.kbd.isKeyPressed(VK_LEFT))
	{
		dorito->move(-(dTime * 0.5f), 0.0f, 0.0f);
	}
	else if (wnd.kbd.isKeyPressed(Keyboard::KeyCode::VK_D) || wnd.kbd.isKeyPressed(VK_RIGHT))
	{
		dorito->move(dTime * 0.5f, 0.0f, 0.0f);
	}

	if (wnd.mouse.isButtonPressed(Mouse::BUTTON_LEFT))
	{
		ErrorLogger::Log(L"", L"Left Button Pressed");
	}
	else if (wnd.mouse.isButtonPressed(Mouse::BUTTON_RIGHT))
	{
		ErrorLogger::Log(L"", L"Right Button Pressed");
	}
	else if (wnd.mouse.isButtonPressed(Mouse::BUTTON_MIDDLE))
	{
		ErrorLogger::Log(L"", L"Middle Button Pressed");
	}

#pragma region Game code
/*
	if (kb.Q)
	{
		if (randomNum == 0)
		{
			didRequest = true;
		}
	}
	else if (kb.E)
	{
		if (randomNum == 1)
		{
			didRequest = true;
		}
	}
	else if (kb.W)
	{
		if (randomNum == 2)
		{
			didRequest = true;
		}
	}
	else if (kb.S)
	{
		if (randomNum == 3)
		{
			didRequest = true;
		}
	}
	else if (kb.A)
	{
		if (randomNum == 4)
		{
			didRequest = true;
		}
	}
	else if (kb.D)
	{
		if (randomNum == 5)
		{
			didRequest = true;
		}
	}

	if (timer.Peek() > 2.5f)
	{
		randomNum = rand() % 6;
		switch (randomNum)
		{
		case 0:
		{
			std::wstring title = L"Dorito Says Rotate Left, Task Number:" + std::to_wstring(taskNum);
			wnd.setTitle(title.c_str());
		}
		break;
		case 1:
		{
			std::wstring title = L"Dorito Says Rotate Right, Task Number:" + std::to_wstring(taskNum);
			wnd.setTitle(title.c_str());
		}
		break;
		case 2:
		{
			std::wstring title = L"Dorito Says Scale Up, Task Number:" + std::to_wstring(taskNum);
			wnd.setTitle(title.c_str());
		}
		break;
		case 3:
		{
			std::wstring title = L"Dorito Says Scale Down, Task Number:" + std::to_wstring(taskNum);
			wnd.setTitle(title.c_str());
		}
		break;
		case 4:
		{
			std::wstring title = L"Dorito Says Move Left, Task Number:" + std::to_wstring(taskNum);
			wnd.setTitle(title.c_str());
		}
		break;
		case 5:
		{
			std::wstring title = L"Dorito Says Move Right, Task Number:" + std::to_wstring(taskNum);
			wnd.setTitle(title.c_str());
		}
		break;
		}
		if (didRequest == false)
		{
			wnd.setTitle(L"YOU FAILED");
			didRequest = true;
		}
		else
		{
			didRequest = false;
			taskNum++;
		}

		timer.Mark();
	}
*/
#pragma endregion

	dorito->transform.position.x = std::clamp(dorito->transform.position.x, -1.5f, 1.5f);
	dorito->transform.position.y = std::clamp(dorito->transform.position.y, -0.30f, 1.15f);

	if (wnd.gamepad.Update())
	{
		float joyX = wnd.gamepad.leftStickX;
		float joyY = wnd.gamepad.leftStickY;
		dorito->move(0.0f, joyY * dTime * 0.5f, 0.0f);
		dorito->move(joyX * dTime * 0.5f, 0.0f, 0.0f);
	}
	dorito->rotate(0.0f, 1.0f, 0.0f, sin(eTime) * 35.0f);
	dorito->setScale(1.0f, 1.0f, 1.0f);

	dorito->update(timer.Peek(), camera);

	floorModel->resetMatrix();

	floorModel->setPos(0.0f, -0.75f, 3.0f);
	floorModel->rotate(1.0f, 0.0f, 0.0f, 85.0f);
	floorModel->setScale(4.0f, 4.0f, 1.0f);

	floorModel->update(timer.Peek(), camera);


	dogModel->resetMatrix();

	dogModel->setPos(-3.0f, .45f, 5.0f);
	dogModel->rotate(0.0f, 1.0f, 0.0f, -25.0f);
	dogModel->setScale(sin(eTime) * 5, sin(eTime) * 5, 0.0f);

	dogModel->update(timer.Peek(), camera);

	bagModel->resetMatrix();

	bagModel->setPos(3.0f, .45f, 5.0f);
	bagModel->rotate(0.0f, 1.0f, 0.0f, 25.0f);
	bagModel->setScale(4.0f, 4.0f, 1.0f);

	bagModel->update(timer.Peek(), camera);

	mtnDewModel->resetMatrix();
	
	mtnDewModel->setPos(sin(eTime) / 1.0f, 2.0f, 8.0f);
	mtnDewModel->rotate(0.0, 0.5f, 1.0f, eTime * 32);
	//mtnDewModel->rotate(0.0, 1.0f, 0.0f, sin(eTime) * 35.0f);
	mtnDewModel->setScale(3.0f, 4.0f, 1.0f);

	mtnDewModel->update(timer.Peek(), camera);
}

void Game::DrawScene()
{
	wnd.getGraphics().Begin(0.0f, 0.0f, 0.5f);

	dogModel->bind(wnd.getGraphics().vertexShader, wnd.getGraphics().pixelShader, catTex);
	dogModel->draw();
	dogModel->unbind();

	bagModel->bind(wnd.getGraphics().vertexShader, wnd.getGraphics().pixelShader, bagTex);
	bagModel->draw();
	bagModel->unbind();
	
	dorito->bind(wnd.getGraphics().vertexShader, wnd.getGraphics().pixelShader, dorTex);
	dorito->draw();
	dorito->unbind();

	floorModel->bind(wnd.getGraphics().vertexShader, wnd.getGraphics().pixelShader, florTex);
	floorModel->draw();
	floorModel->unbind();

	mtnDewModel->bind(wnd.getGraphics().vertexShader, wnd.getGraphics().pixelShader, mtnTex);
	mtnDewModel->draw();
	mtnDewModel->unbind();

	wnd.getGraphics().End();
}
