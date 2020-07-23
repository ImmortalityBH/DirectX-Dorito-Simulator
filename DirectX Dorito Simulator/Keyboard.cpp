#include "Keyboard.h"
#include "Keyboard.h"

Keyboard::Keyboard()
{
	for (std::size_t i = 0; i < std::size(keyStates); i++)
	{
		keyStates[i] = false;//init to false
	}
}

Keyboard& Keyboard::get()
{
	static Keyboard instance;
	return instance;
}

bool Keyboard::isKeyPressed(const unsigned char keyCode)
{
	return keyStates[keyCode];
}

void Keyboard::OnKeyPressed(const unsigned char keyCode)
{
	keyStates[keyCode] = true;
}

void Keyboard::OnKeyReleased(const unsigned char keyCode)
{
	keyStates[keyCode] = false;
}

void Keyboard::OnChar(const unsigned char keyCode)
{
	if (charBuffer.size() < 256)
	{
		charBuffer.push_back(keyCode);
	}
	else
	{
		charBuffer.pop_front();
	}
}

void Keyboard::clearCharBuffer()
{
	charBuffer.clear();
}

std::list<unsigned char>& Keyboard::getCharBuffer()
{
	return charBuffer;
}
