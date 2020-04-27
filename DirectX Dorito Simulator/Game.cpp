#include "Game.h"
#include <string>
#include <vector>

Game::Game()
	: wnd(1280, 720, L"DirectX Dorito Simulator: Deluxe Edition"),
	  camera(wnd)
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
	floorModel = std::make_unique<Model>(wnd.Gfx(), L"res/img/floor.dds");
	dogModel = std::make_unique<Model>(wnd.Gfx(), L"res/img/dog.dds");
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
		Vertex(0.5f, -0.5f, 0.0f, 1.0f, 1.0f),
		Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 1.0f),
		Vertex(0.5f, 0.5f, 0.0f, 1.0f, 0.0f),
	};
	std::vector<UINT> floorIndices = 
	{
		0, 1, 2,
		3, 1, 0,
	};

	model->create(vertices, L"VertexShader.cso", L"PixelShader.cso");
	floorModel->create(floorVertices, floorIndices, L"VertexShader.cso", L"PixelShader.cso");
	dogModel->create(floorVertices, floorIndices, L"VertexShader.cso", L"PixelShader.cso");
}

void Game::UpdateScene()
{
	float timeSinceStart = timer.Mark();
	float dt = timeSinceStart - oldTimeSinceStart;
	oldTimeSinceStart = timeSinceStart;

	auto kb = wnd.kbd->GetState();
	auto ms = wnd.mouse->GetState();

	if (!wnd.aud->Update())
	{
		
	}
	
	//std::wstring title = L"Mouse Pos (" + std::to_wstring(ms.x) + L", " + std::to_wstring(ms.y) + L")";
	//wnd.setTitle(title.c_str());
	//std::wstring title = L"Deltatime: " + std::to_wstring(dt);
	//wnd.setTitle(title.c_str());

	if (ms.leftButton)
	{
		MessageBox(nullptr, L"COOCHIE", L"LOL", MB_OK);
	}
	//std::wstring title =  L"Elapsed Time: " + std::to_wstring((int)round(timer.Peek()));
	//wnd.setTitle(title.c_str());
	camera.update(dt, wnd);

	model->resetMatrix();

	model->move(0.0f, 0.35f, 1.5f);
	model->rotate(0.0f, 1.0f, 0.0f, 0.0f);
	model->scale(1.0f, 1.0f, 1.0f);

	model->update(timer.Peek(), camera);

	floorModel->resetMatrix();

	floorModel->move(0.0f, -0.75f, 3.0f);
	floorModel->rotate(1.0f, 0.0f, 0.0f, 85.0f);
	floorModel->scale(4.0f, 4.0f, 1.0f);

	floorModel->update(timer.Peek(), camera);

	dogModel->resetMatrix();

	dogModel->move(-3.0f, .45f, 5.0f);
	dogModel->rotate(0.0f, 1.0f, 0.0f, -25.0f);
	dogModel->scale(4.0f, 4.0f, 1.0f);

	dogModel->update(timer.Peek(), camera);
}

void Game::DrawScene()
{
	wnd.Gfx().Begin(0.0f, 0.0f, 0.5f);

	dogModel->bind();
	dogModel->draw();
	dogModel->unbind();

	model->bind();
	model->draw();
	model->unbind();

	floorModel->bind();
	floorModel->draw();
	floorModel->unbind();

	wnd.Gfx().End();
}
