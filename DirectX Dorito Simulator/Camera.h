#pragma once

#include "Window.h"
#include <DirectXMath.h>

class Camera
{
public:
	Camera();
	~Camera();

private:
	DirectX::XMMATRIX projection;
};

