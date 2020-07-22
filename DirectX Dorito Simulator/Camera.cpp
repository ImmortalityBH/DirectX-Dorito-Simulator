#include "Camera.h"

using namespace DirectX;

Camera::Camera(Window& wnd)
	: pMouse(wnd.mouse.get())
{
	position = XMVectorSet(0.0f, 0.0f, -0.5f, 0.0f);
	target = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), (float)wnd.getWidth() / wnd.getHeight(), 0.01f, 100.0f);
}

void Camera::update(float dt, Window& wnd)
{
	auto kb = wnd.kbd->GetState();

	view = XMMatrixLookAtLH(position, target, up);
}
