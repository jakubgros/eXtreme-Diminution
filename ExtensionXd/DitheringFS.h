#pragma once
#include "types.h"
#include <functional>
#include "ColorFinder.h"

class DitheringFs
{
public:
	DitheringFs() = default;
	~DitheringFs() = default;
	void operator() (Pixmap* pixmap, const Palette* palette, const std::function<Rgb(const Rgb&)>& findColorInPalette = nullptr);
private:
	Pixmap* pixmap_ = nullptr;
	const Palette* palette_ = nullptr;
	std::function<Rgb(const Rgb&)> findColorInPalette_ = nullptr;
	Pixmap error_;
	ColorFinder defaultFinder_;
private:
	static void adjustTo8Bits(Rgb& newColor);
	static void adjustRange(int& component);
	void assignResources(Pixmap* pixmap, const Palette* palette, const std::function<Rgb(const Rgb&)> findColorInPalette);
	void checkResources() const;
	void initErrorTable();
	void resizeErrorTable();
	void clearErrorTable();
	void dither();
	Rgb findNewColor(const Rgb& currentColor);
	void updateQuantError(const size_t x, const size_t y, const Rgb& newColor);
};

namespace dfs
{
	extern DitheringFs ditheringFs;
}