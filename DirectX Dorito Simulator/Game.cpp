#include "Game.h"
#include <string>

#include <ctime>
#include <random>

#define NUM_OF_SAYS 3

#define SPEED 0.1f

Game::Game()
	: wnd(800, 600, L"DirectX Dorito Simulator")
{
	srand(time(NULL));
}

int Game::run(HINSTANCE hInstance)
{
	wnd.init(hInstance);
	MSG msg = {};
	tri = std::make_unique<Triangle>(wnd.Gfx());
	tri->Create();
	//tri->Move(0.0f, 0.0f, -4.0f);
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

void Game::UpdateScene()
{
	auto kb = wnd.kbd->GetState();

	float timeSinceStart = timer.Mark();
	float dt = timeSinceStart - oldTimeSinceStart;
	oldTimeSinceStart = timeSinceStart;

	if (kb.Q)
	{
		tri->Move(timer.Mark(), 0);
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
	//std::wstring title =  L"Elapsed Time: " + std::to_wstring((int)round(timer.Peek()));
	//wnd.setTitle(title.c_str());
	
}

void Game::DrawScene()
{
	wnd.Gfx().Begin(0.0f, 0.0f, 0.5f);
	tri->Update();
	tri->Draw();
	wnd.Gfx().End();
}
