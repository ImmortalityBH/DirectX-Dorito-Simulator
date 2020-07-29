#pragma once

#include "Window/Window.h"
#include <DirectXMath.h>

namespace DX = DirectX;

class Camera
{
public:
	Camera(Window& wnd);

	void update(float dt, Window& wnd);

	DX::XMMATRIX getView() const { return view; }
	DX::XMMATRIX getProjection() const { return projection; }
private:
	DX::XMMATRIX projection;
	DX::XMMATRIX view;

	DX::XMVECTOR position;
	DX::XMVECTOR target;
	DX::XMVECTOR up;

	const float speed = 0.01f;
};

