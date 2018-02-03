#include "XdToBmpConverter.h"
#include "Types.h"

XdToBmpConverter::XdToBmpConverter(ImgWithParam* img, ColorMode mode) :
	Converter(img),
	mode(mode)
{

}

void XdToBmpConverter::convert()
{
	int y2 = img->height - 1;
	for (int y = 0; y < y2; ++y, --y2)
	{
		for (int x = 0; x < img->width; ++x)
			std::swap(img->pixmap[x][y], img->pixmap[x][y2]);
	}
}