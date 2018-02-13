#include "PixmapToStringConverter.h"
#include <string>

PixmapToStringConverter::PixmapToStringConverter(Pixmap* pixmap, Palette* palette) :
	pixmap_(pixmap),
	palette_(palette),
	sPixmap_("")
{

}

std::string PixmapToStringConverter::convert()
{
	for (int y = 0; y < (*pixmap_)[0].size(); ++y)
		for (int x = 0; x < (*pixmap_).size(); ++x)
			addColorFromPalette((*pixmap_)[x][y]);
	return sPixmap_;
}

void PixmapToStringConverter::addColorFromPalette(const Rgb& pixel)
{
	size_t colorNumber = findInPalette(pixel);
	checkIfFound(colorNumber);
	addColor(colorNumber);
}

size_t PixmapToStringConverter::findInPalette(const Rgb& pixel)
{
	for (size_t color = 0; color < (*palette_).size(); ++color)
	{
		if ((*palette_)[color] == pixel)
			return color;
	}
	return UINT32_MAX;
}

void PixmapToStringConverter::addColor(size_t color)
{
	twosComplement(color);
	sPixmap_ += std::to_string(color);
}

void PixmapToStringConverter::twosComplement(const size_t color)
{
	if (color < 10)
		sPixmap_ += '0';
}

void PixmapToStringConverter::checkIfFound(size_t colorNumber) const
{
	if (colorNumber == UINT32_MAX)
		throw std::logic_error("PixmapToStringConverter - brak koloru w palecie");
}

std::string PixmapToStringConverter::getPixmap() const
{
	return sPixmap_;
}
