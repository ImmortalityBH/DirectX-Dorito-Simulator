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
public:
	Mouse() {}
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	~Mouse() = default;

	MousePos& getPos();

	void OnMouseMove(int x, int y);
	void OnLeftPressed(int x, int y);
	void OnRightPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightReleased(int x, int y);
};

