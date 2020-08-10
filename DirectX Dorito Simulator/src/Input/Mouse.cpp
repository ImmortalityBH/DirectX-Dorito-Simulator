#include "Input/Mouse.h"

Mouse::Mouse()
	: pos{0,0}, wheelDelta(0.0f)
{
}

const Mouse::MousePos& Mouse::getPos() const
{
	return pos;
}

const float& Mouse::getWheelDelta() const
{
	return wheelDelta;
}

bool Mouse::isButtonPressed(MouseButton button)
{
	switch (button)
	{
	case BUTTON_LEFT:
		return leftButton;
		break;
	case BUTTON_RIGHT:
		return rightButton;
		break;
	case BUTTON_MIDDLE:
		return middleButton;
		break;
	}
}

void Mouse::OnMouseMove(int x, int y)
{
	pos.x = x;
	pos.y = y;
}

void Mouse::OnLeftPressed(int x, int y)
{
	pos.x = x;
	pos.y = y;
	leftButton = true;
}

void Mouse::OnRightPressed(int x, int y)
{
	pos.x = x;
	pos.y = y;
	rightButton = true;
}

void Mouse::OnMiddlePressed(int x, int y)
{
	pos.x = x;
	pos.y = y;
	middleButton = true;
}

void Mouse::OnLeftReleased(int x, int y)
{
	pos.x = x;
	pos.y = y;
	leftButton = false;
}

void Mouse::OnRightReleased(int x, int y)
{
	pos.x = x;
	pos.y = y;
	rightButton = false;
}

void Mouse::OnMiddleReleased(int x, int y)
{
	pos.x = x;
	pos.y = y;
	middleButton = false;
}

void Mouse::OnWheelMove(int x, int y, float wheelDelta)
{
	pos.x = x;
	pos.y = y;
	this->wheelDelta = wheelDelta;
}

void Mouse::updateBuffer(MouseButton& button)
{
	if (buffer.size() <= bufferLimit)
	{
		buffer.push_front(button);
	}
	else
	{
		buffer.push_front(button);
		buffer.pop_back();
	}
}

void Mouse::flush()
{
	buffer.clear();
}
