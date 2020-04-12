#include "Game.h"

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
	if (kb.A)
	{
		MessageBox(nullptr, L"Pressed a button", L"Alert", MB_OK);
	}
}

void Game::DrawScene()
{
	wnd.Gfx().Begin(1.0f, 0.0f, 0.0f);
	tri->Draw();
	wnd.Gfx().End();
}
