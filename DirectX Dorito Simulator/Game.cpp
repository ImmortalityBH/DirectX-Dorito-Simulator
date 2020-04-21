#include "Game.h"
#include <string>

Game::Game()
	: wnd(800, 600, L"DirectX Dorito Simulator")
{
}

Game::~Game()
{
}

int Game::run(HINSTANCE hInstance)
{
	wnd.init(hInstance);
	MSG msg = {};
	Init();
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

void Game::Init()
{
	model = std::make_unique<Model>(wnd.Gfx(), L"res/img/dorito.dds");
	model->create();
}

void Game::UpdateScene()
{
	auto kb = wnd.kbd->GetState();

	float timeSinceStart = timer.Mark();
	float dt = timeSinceStart - oldTimeSinceStart;
	oldTimeSinceStart = timeSinceStart;

	auto ms = wnd.mouse->GetState();
	std::wstring title = L"Mouse Pos (" + std::to_wstring(ms.x) + L", " + std::to_wstring(ms.y) + L")";
	wnd.setTitle(title.c_str());
	if (ms.leftButton)
	{
		MessageBox(nullptr, L"COOCHIE", L"LOL", MB_OK);
	}
	//std::wstring title =  L"Elapsed Time: " + std::to_wstring((int)round(timer.Peek()));
	//wnd.setTitle(title.c_str());
	model->update();
}

void Game::DrawScene()
{
	wnd.Gfx().Begin(0.0f, 0.0f, 0.5f);
	model->bindShaders();
	model->draw();
	model->unbindShaders();
	wnd.Gfx().End();
}
