#include "Game.h"
#include <string>

Game::Game()
	: wnd(800, 600, L"DirectX Dorito Simulator")
{
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

	//std::wstring title =  L"Elapsed Time: " + std::to_wstring((int)round(timer.Peek()));
	//wnd.setTitle(title.c_str());
	
}

void Game::DrawScene()
{
	wnd.Gfx().Begin(0.0f, 0.0f, 0.5f);
	//tri->Update();
	//tri->Draw();
	wnd.Gfx().End();
}
