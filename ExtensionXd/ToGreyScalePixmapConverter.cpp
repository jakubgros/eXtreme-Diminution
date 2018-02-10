#include "ToGreyScalePixmapConverter.h"
#include "Types.h"
#include <iostream>
#include "DitheringFS.h"

ToGreyScalePixmapConverter::ToGreyScalePixmapConverter(ImgWithParam* imgWithParam) :
	BmpToXdConverter(imgWithParam)
{

}

void ToGreyScalePixmapConverter::convert()
{
	checkResources();
	dfs::ditheringFs(&img->pixmap, &img->imposedGreyPalette, findClosestPaletteColor);
}

void ToGreyScalePixmapConverter::checkResources() const
{
	if (img == nullptr)
		throw std::logic_error("ToGreyScalePixmapConverter - nullptr resources");
}

Rgb ToGreyScalePixmapConverter::findClosestPaletteColor(const Rgb& color)
{
	Rgb newColor = rgbToYuv(color);
	switch (newColor.r % 4)
	{
	case 0:
		break;
	case 1:
		newColor = newColor - Rgb{ 1,1,1 };
		break;
	case 2:
		newColor = newColor - Rgb{ 2,2,2 };
		break;
	case 3:
		if (newColor.r == 255)
			newColor = Rgb{ 252, 252, 252 };
		else
			newColor = newColor + Rgb{ 1,1,1 };
		break;
	default:
		break;
	}
	return newColor;
}

Rgb ToGreyScalePixmapConverter::rgbToYuv(Rgb pixel)
{
	const int newColor = pixel.r*0.299 + pixel.g*0.587 + pixel.b*0.114;
	pixel.r = pixel.g = pixel.b = newColor;
	return pixel;
}