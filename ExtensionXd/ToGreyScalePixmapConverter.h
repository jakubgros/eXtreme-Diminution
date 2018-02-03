#pragma once
#include "Converter.h"

struct Rgb;

class ToGreyScalePixmapConverter : public BmpToXdConverter
{
public:
	explicit ToGreyScalePixmapConverter(ImgWithParam* imgWithParam);
	virtual ~ToGreyScalePixmapConverter() = default;
	virtual void convert() override;
private:
	Rgb findClosestPaletteColor(Rgb pixel);
	Rgb colorToYUV(Rgb pixel);
private:
	ToGreyScalePixmapConverter(const ToGreyScalePixmapConverter&);
	ToGreyScalePixmapConverter& operator=(const ToGreyScalePixmapConverter&);
};