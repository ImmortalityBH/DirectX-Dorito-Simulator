#pragma once

struct MousePos
{
	int x, y;
};

class Mouse
{
private:
	MousePos pos;
	bool leftButton = false;
	bool rightButton = false;
	bool middleButton = false;
	float wheelDelta;
	Mouse();
public:
	enum MouseButton
	{
		BUTTON_LEFT = 0,
		BUTTON_RIGHT,
		BUTTON_MIDDLE,
	};
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	~Mouse() = default;

	static Mouse& get();

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
};

