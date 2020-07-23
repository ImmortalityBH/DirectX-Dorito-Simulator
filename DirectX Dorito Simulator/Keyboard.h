#pragma once
class Keyboard
{
public:
	
	static Keyboard& get();

	~Keyboard() = default;
private:
	Keyboard() {};
};

