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
	if (kb.W)
	{

	}
	else if (kb.S)
	{

	}
	else if (kb.A)
	{

	}
	else if (kb.D)
	{

	}
	if (timer.Peek() > 6.0f)
	{
		timer.Reset();
	}
	std::wstring title =  L"Elapsed Time: " + std::to_wstring((int)round(timer.Peek()));
	wnd.setTitle(title.c_str());
}

void Game::DrawScene()
{
	wnd.Gfx().Begin(0.0f, 0.0f, 0.5f);
	tri->Draw();
	wnd.Gfx().End();
}
