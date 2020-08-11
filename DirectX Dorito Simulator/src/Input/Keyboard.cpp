#include "Input/Keyboard.h"

Keyboard::Keyboard()
{
	for (std::size_t i = 0; i < keyStates.size(); i++)
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
	if (charBuffer.size() <= charBufferLimit)
	{
		charBuffer.push_back(keyCode);
	}
	else
	{
		charBuffer.push_back(keyCode);
		charBuffer.pop_front();
	}
}

inline void Keyboard::setAutoRepeatKeys(bool value)
{
	autoRepeatKeys = value;
}

inline void Keyboard::setAutoRepeatChars(bool value)
{
	autoRepeatChars = value;
}

inline void Keyboard::flush()
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
