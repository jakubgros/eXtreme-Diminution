#include "PixmapToStringConverter.h"

PixmapToStringConverter::PixmapToStringConverter(ImgWithParam* img) :
	img(img),
	pixmap("")
{

}

std::string PixmapToStringConverter::convert()
{
	for (int y = 0; y < img->height; ++y)
		for (int x = 0; x < img->width; ++x)
			addColorFromPalette(img->pixmap[x][y]);
	return pixmap;
}

void PixmapToStringConverter::addColorFromPalette(const Rgb& pixel)
{
	switch (img->colorMode)
	{
	case GREY_SCALE:
		addFromGreyScale(pixel);
		break;
	case DEDICATED:
		addFromDedicated(pixel);
		break;
	case IMPOSED:
		addFromImposed(pixel);
		break;
	}
}

void PixmapToStringConverter::addFromGreyScale(const Rgb& pixel)
{
	for (int color = 0; color < img->imposedGreyPalette.size(); ++color)
	{
		if (img->imposedGreyPalette[color] == pixel)
		{
			if (color < 10)
				pixmap += '0';
			pixmap += std::to_string(color);
			break;
		}
	}
}

void PixmapToStringConverter::addFromImposed(const Rgb& pixel)
{
	for (int color = 0; color < img->imposedColorPalette.size(); ++color)
	{
		if (img->imposedColorPalette[color] == pixel)
		{
			if (color < 10)
				pixmap += '0';
			pixmap += std::to_string(color);
			break;
		}
	}
}

void PixmapToStringConverter::addFromDedicated(const Rgb& pixel)
{
	for (int color = 0; color < img->dedicatedColorPalette.size(); ++color)
	{
		if (img->dedicatedColorPalette[color] == pixel)
		{
			if (color < 10)
				pixmap += '0';
			pixmap += std::to_string(color);
			break;
		}
	}
}

std::string PixmapToStringConverter::getPixmap()
{
	return pixmap;
}
