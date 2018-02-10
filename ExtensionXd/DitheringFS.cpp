#include "DitheringFS.h"

void DitheringFs::operator()(Pixmap* pixmap, const Palette* palette, const std::function<Rgb(const Rgb&)> findColorInPalette)
{
	pixmap_ = pixmap;
	palette_ = palette;
	findColorInPalette_ = findColorInPalette;
	checkResources();
	dither();
}

void DitheringFs::checkResources() const
{
	if (pixmap_ == nullptr || palette_ == nullptr)
		throw std::logic_error("DitheringFs = nullptr resources");
}

void DitheringFs::dither()
{
	for (size_t y = 0; y < (*pixmap_)[0].size(); ++y)
	{
		for (size_t x = 0; x < (*pixmap_).size(); ++x)
		{
			const Rgb newColor = findNewColor((*pixmap_)[x][y]);
			updateQuantError(x,y, newColor);
			(*pixmap_)[x][y] = newColor;
		}
	}
}

Rgb DitheringFs::findNewColor(const Rgb& currentColor) const
{
	Rgb newColor{};
	if (findColorInPalette_)
		newColor = findColorInPalette_(currentColor);
	else
		newColor = findClosestPaletteColor(currentColor);
	return newColor;
}

Rgb DitheringFs::rgb2Yuv(const Rgb& color)
{
	const int yuvColor = color.r*0.299 + color.g*0.587 + color.b*0.114;
	return Rgb{ yuvColor, yuvColor, yuvColor };
}

Rgb DitheringFs::findClosestPaletteColor(const Rgb& color) const
{
	Rgb newColor = rgb2Yuv(color);
	double bestDistance = countDistance(color, (*palette_)[0]);
	unsigned bestColor = 0;
	for (size_t i = 1; i < (*palette_).size(); ++i)
	{
		const double currentDistance = countDistance(color, (*palette_)[i]);
		if (currentDistance < bestDistance)
		{
			bestDistance = currentDistance;
			bestColor = i;
		}
	}
	return  (*palette_)[bestColor];
}

double DitheringFs::countDistance(const Rgb& color1, const Rgb& color2)
{
	return pow((color1.r - color2.r)*0.299, 2)
		+ pow((color1.g - color2.g)*0.587, 2)
		+ pow((color1.b - color2.b)*0.114, 2);
}

void DitheringFs::updateQuantError(const size_t x, const size_t y, const Rgb& newColor)
{
	const Rgb quantError = (*pixmap_)[x][y] - newColor;
	if (x < (*pixmap_).size() - 1)
		(*pixmap_)[x + 1][y] = (*pixmap_)[x + 1][y] + 7. / 16 * quantError;
	if (x > 0 && y < (*pixmap_)[0].size() - 1)
		(*pixmap_)[x - 1][y + 1] = (*pixmap_)[x - 1][y + 1] + 3. / 16 * quantError;
	if (y < (*pixmap_)[0].size() - 1)
		(*pixmap_)[x][y + 1] = (*pixmap_)[x][y + 1] + 5. / 16 * quantError;
	if (x < (*pixmap_).size() - 1 && y < (*pixmap_)[0].size() - 1)
		(*pixmap_)[x + 1][y + 1] = (*pixmap_)[x + 1][y + 1] + 1. / 16 * quantError;
}

DitheringFs dfs::ditheringFs;