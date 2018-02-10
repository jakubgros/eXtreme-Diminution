#pragma once
#include "Converter.h"

struct Rgb;

class ToGreyScalePixmapConverter : public BmpToXdConverter
{
public:
	explicit ToGreyScalePixmapConverter(ImgWithParam* imgWithParam);
	~ToGreyScalePixmapConverter() = default;
	void convert();
private:
	static Rgb findClosestPaletteColor(const Rgb& color);
	static Rgb rgbToYuv(Rgb pixel);
	void checkResources() const;
	ToGreyScalePixmapConverter(const ToGreyScalePixmapConverter&);
	ToGreyScalePixmapConverter& operator=(const ToGreyScalePixmapConverter&);
};