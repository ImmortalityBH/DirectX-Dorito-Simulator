#include "Mouse.h"

MousePos& Mouse::getPos()
{
	return pos;
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
