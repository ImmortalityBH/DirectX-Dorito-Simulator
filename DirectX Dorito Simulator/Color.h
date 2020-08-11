#pragma once

class Color
{
public:
	constexpr Color() 
		: r(0), g(0), b(0), a(0)
	{}
	constexpr Color(unsigned char r, unsigned char g, unsigned char b)
		: r(r), g(g), b(b), a(255)
	{}
	constexpr Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		: r(r), g(g), b(b), a(a)
	{}
	constexpr Color(const Color& color)
		: r(color.r), g(color.g), b(color.b), a(color.a)
	{
	}
	Color& operator=(const Color& color)
	{
		this->r = color.r;
		this->g = color.g;
		this->b = color.b;
		this->a = color.a;
		return *this;
	}
private:
	unsigned char r, g, b, a;
};

namespace Colors
{
	static constexpr Color MakeRGB(unsigned char r, unsigned char g, unsigned char b)
	{
		return Color(r, g, b);
	}
	static constexpr Color White = MakeRGB(255u, 255u, 255u);
	static constexpr Color Black = MakeRGB(0u, 0u, 0u);
	static constexpr Color Gray = MakeRGB(0x80u, 0x80u, 0x80u);
	static constexpr Color LightGray = MakeRGB(0xD3u, 0xD3u, 0xD3u);
	static constexpr Color Red = MakeRGB(255u, 0u, 0u);
	static constexpr Color Green = MakeRGB(0u, 255u, 0u);
	static constexpr Color Blue = MakeRGB(0u, 0u, 255u);
	static constexpr Color Yellow = MakeRGB(255u, 255u, 0u);
	static constexpr Color Cyan = MakeRGB(0u, 255u, 255u);
	static constexpr Color Magenta(255, 0, 255);
}