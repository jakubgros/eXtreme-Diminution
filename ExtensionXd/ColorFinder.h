#pragma once
#include "types.h"

class ColorFinder
{
public:
	ColorFinder();
	~ColorFinder() = default;
	Rgb find(const Rgb color, const Palette* palette);
private:
	const Palette* palette_;
	Rgb colorToFind_;
	double bestDistance_;
	unsigned bestColorNumber_;
private:
	static double countDistance(const Rgb& color1, const Rgb& color2);
	void assignResources(const Rgb color, const Palette* palette);
	void checkResources() const;
	void findBestColor();
	void initBests();
	void searchInPalette();
	void updateIfBest(const size_t colorNumber);
	void overwriteBests(const size_t colorNumber, double);
private:
	ColorFinder(const ColorFinder&);
	ColorFinder& operator=(const ColorFinder&);
};
