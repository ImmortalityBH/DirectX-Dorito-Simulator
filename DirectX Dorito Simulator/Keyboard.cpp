#include "Keyboard.h"
#include "Keyboard.h"

Keyboard& Keyboard::get()
{
	static Keyboard instance;
	return instance;
}
