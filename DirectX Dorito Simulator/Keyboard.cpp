#include "Keyboard.h"
#include "Keyboard.h"

Keyboard::Keyboard()
{
	for (std::size_t i = 0; i < std::size(keyStates); i++)
	{
		keyStates[i] = false;//init to false
	}
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
		charBuffer.push_back(keyCode);
		charBuffer.pop_front();
	}
}

inline void Keyboard::clearCharBuffer()
{
	charBuffer.clear();
}

std::deque<unsigned char>& Keyboard::getCharBuffer()
{
	return charBuffer;
}

std::wstring Keyboard::getCharAsString()
{
	std::wstring charStr = L"";
	for (std::size_t i = 0; i < charBuffer.size(); i++)
	{
		charStr.append(1, static_cast<wchar_t>(charBuffer[i]));
	}
	return charStr;
}