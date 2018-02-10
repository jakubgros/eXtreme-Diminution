#pragma once
#include "Types.h"
#include <functional>

class DitheringFs
{
public:
	void operator() (Pixmap* pixmap, const Palette* palette, std::function<Rgb(const Rgb&)> findColorInPalette = nullptr);
private:
	Pixmap* pixmap_ = nullptr;
	const Palette* palette_ = nullptr;
	std::function<Rgb(const Rgb&)> findColorInPalette_ = nullptr;
private:
	static Rgb rgb2Yuv(const Rgb& color);
	static double countDistance(const Rgb& color1, const Rgb& color2);
	void updateQuantError(const size_t x, const size_t y, const Rgb& newColor);
	void checkResources() const;
	void dither();
	Rgb findNewColor(const Rgb& currentColor) const;
	Rgb findClosestPaletteColor(const Rgb& color) const;
};

namespace dfs
{
	extern DitheringFs ditheringFs;
}