#include "DitheringFS.h"

void DitheringFs::operator()(Pixmap* pixmap, const Palette* palette, const std::function<Rgb(const Rgb&)>& findColorInPalette)
{
	assignResources(pixmap, palette, findColorInPalette);
	checkResources();
	initErrorTable();
	dither();
}

void DitheringFs::assignResources(Pixmap* pixmap, const Palette* palette, const std::function<Rgb(const Rgb&)> findColorInPalette)
{
	pixmap_ = pixmap;
	palette_ = palette;
	findColorInPalette_ = findColorInPalette;
}

void DitheringFs::checkResources() const
{
	if (pixmap_ == nullptr || palette_ == nullptr)
		throw std::logic_error("DitheringFs = nullptr resources");
}

void DitheringFs::initErrorTable()
{
	resizeErrorTable();
	clearErrorTable();
}

void DitheringFs::resizeErrorTable()
{
	error_.resize((*pixmap_).size());
	for (size_t i = 0; i < (*pixmap_).size(); ++i)
		error_[i].resize((*pixmap_)[0].size());
}

void DitheringFs::clearErrorTable()
{
	for (size_t x = 0; x < (*pixmap_).size(); ++x)
		for (size_t y = 0; y < (*pixmap_)[0].size(); ++y)
			(error_)[x][y] = { 0,0,0 };
}


void DitheringFs::dither()
{
	for (size_t y = 0; y < (*pixmap_)[0].size(); ++y)
	{
		for (size_t x = 0; x < (*pixmap_).size(); ++x)
		{
			Rgb newColor = findNewColor((*pixmap_)[x][y] + error_[x][y]);
			updateQuantError(x, y, newColor);
			adjustTo8Bits(newColor);
			(*pixmap_)[x][y] = newColor;
		}
	}
}

Rgb DitheringFs::findNewColor(const Rgb& currentColor)
{
	Rgb newColor{};
	if (findColorInPalette_)
		newColor = findColorInPalette_(currentColor);			//TODO dac wyzej, zeby dla kazdego xy nie sprawdzalo
	else
		newColor = defaultFinder_.find(currentColor, palette_);
	return newColor;
}

void DitheringFs::updateQuantError(const size_t x, const size_t y, const Rgb& newColor)
{
	const Rgb quantError = (*pixmap_)[x][y] - newColor;
	if (x < (error_).size() - 1)
		(error_)[x + 1][y] = (error_)[x + 1][y] + 7. / 16 * quantError;
	if (x > 0 && y < (error_)[0].size() - 1)
		(error_)[x - 1][y + 1] = (error_)[x - 1][y + 1] + 3. / 16 * quantError;
	if (y < (error_)[0].size() - 1)
		(error_)[x][y + 1] = (error_)[x][y + 1] + 5. / 16 * quantError;
	if (x < (error_).size() - 1 && y < (error_)[0].size() - 1)
		(error_)[x + 1][y + 1] = (error_)[x + 1][y + 1] + 1. / 16 * quantError;
}

void DitheringFs::adjustTo8Bits(Rgb& newColor)
{
	adjustRange(newColor.r);
	adjustRange(newColor.g);
	adjustRange(newColor.b);
}

void DitheringFs::adjustRange(int& component)
{
	if (component > 255)
		component = 255;
	else
		if (component < 0)
			component = 0;
}

DitheringFs dfs::ditheringFs;