#include "ColorFinder.h"
#include <algorithm>

ColorFinder::ColorFinder() :
	palette_(nullptr),
	colorToFind_({}),
	bestDistance_(0),
	bestColorNumber_(0)
{

}

Rgb ColorFinder::find(const Rgb color, const Palette* palette)
{
	assignResources(color, palette);
	checkResources();
	findBestColor();
	return  (*palette_)[bestColorNumber_];
}

void ColorFinder::assignResources(const Rgb color, const Palette* palette)
{
	palette_ = palette;
	colorToFind_ = color;
}

void ColorFinder::checkResources() const
{
	if (palette_ == nullptr)
		throw std::logic_error("ColorFined - nullptr resources");
}

void ColorFinder::findBestColor()
{
	initBests();
	searchInPalette();
}

void ColorFinder::initBests()
{
	bestDistance_ = countDistance(colorToFind_, (*palette_)[0]);
	bestColorNumber_ = 0;
}

void ColorFinder::searchInPalette()
{
	for (size_t colorNumber = 1; colorNumber < (*palette_).size(); ++colorNumber)
		updateIfBest(colorNumber);
}

void ColorFinder::updateIfBest(const size_t colorNumber)
{
	const double currentDistance = countDistance(colorToFind_, (*palette_)[colorNumber]);
	if (currentDistance < bestDistance_)
		overwriteBests(colorNumber, currentDistance);
}

double ColorFinder::countDistance(const Rgb& color1, const Rgb& color2)
{
		return pow((color1.r - color2.r)*0.299, 2)
		+ pow((color1.g - color2.g)*0.587, 2)
		+ pow((color1.b - color2.b)*0.114, 2);
}

void ColorFinder::overwriteBests(const size_t colorNumber, const double currentDistance)
{
	bestDistance_ = currentDistance;
	bestColorNumber_ = colorNumber;
}