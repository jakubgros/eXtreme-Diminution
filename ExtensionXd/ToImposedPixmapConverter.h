#pragma once
#include "Converter.h"

struct Rgb;

class ToImposedPixmapConverter : public BmpToXdConverter
{
public:
	explicit ToImposedPixmapConverter(ImgWithParam* imgWithParam);
	virtual ~ToImposedPixmapConverter() = default;
	virtual void convert() override;
private:
	Rgb findClosestPaletteColor(Rgb pixel);
	double countDistance(Rgb pixel1, Rgb pixel2);
private:
	ToImposedPixmapConverter(const ToImposedPixmapConverter&);
	ToImposedPixmapConverter& operator=(const ToImposedPixmapConverter&);
};