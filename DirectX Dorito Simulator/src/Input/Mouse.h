#pragma once

#include <deque>

class Mouse
{
public:
	enum MouseButton
	{
		BUTTON_LEFT = 0,
		BUTTON_RIGHT,
		BUTTON_MIDDLE,
	};
private:
	struct MousePos
	{
		int x, y;
	} pos;
	bool leftButton = false;
	bool rightButton = false;
	bool middleButton = false;
	float wheelDelta;
	std::deque<MouseButton> buffer;
	static constexpr std::size_t bufferLimit = 52;
public:	
	Mouse();
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	~Mouse() = default;

	const MousePos& getPos() const;
	const float& getWheelDelta() const;

	bool isButtonPressed(MouseButton button);

	void OnMouseMove(int x, int y);
	void OnLeftPressed(int x, int y);
	void OnRightPressed(int x, int y);
	void OnMiddlePressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightReleased(int x, int y);
	void OnMiddleReleased(int x, int y);
	void OnWheelMove(int x, int y, float wheelDelta);

	int GetX() const { return pos.x; }
	int GetY() const { return pos.y; }

	void updateBuffer(MouseButton& button);
	void flush();
};

