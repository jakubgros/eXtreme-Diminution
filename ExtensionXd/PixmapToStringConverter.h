#pragma once
#include "types.h"

class PixmapToStringConverter
{
public:
	explicit PixmapToStringConverter(Pixmap* pixmap, Palette* palette);
	~PixmapToStringConverter() = default;
	std::string convert();
	std::string getPixmap() const;
private:
	Pixmap* pixmap_;
	Palette* palette_;
	std::string sPixmap_;
private:
	void addColorFromPalette(const Rgb& pixel);
	size_t findInPalette(const Rgb& pixel);
	void addColor(size_t color);
	void twosComplement(size_t color);
	void checkIfFound(size_t colorNumber) const;
private:
	PixmapToStringConverter(const PixmapToStringConverter&);
	PixmapToStringConverter& operator=(const PixmapToStringConverter&);
};