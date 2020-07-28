#pragma once

class Bindable
{
public:

	virtual void bind() = 0;
	virtual void unbind() = 0;
private:
};