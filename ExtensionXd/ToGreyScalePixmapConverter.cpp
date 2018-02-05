#include "ToGreyScalePixmapConverter.h"
#include "Types.h"


ToGreyScalePixmapConverter::ToGreyScalePixmapConverter(ImgWithParam* imgWithParam) :
	BmpToXdConverter(imgWithParam)
{

}

void ToGreyScalePixmapConverter::convert()
{
	for (int y = 0; y < img->height; ++y)
	{
		for (int x = 0; x < img->width; ++x)
		{
			Rgb oldPixel = img->pixmap[x][y];
			oldPixel = colorToYUV(oldPixel);
			Rgb newPixel = findClosestPaletteColor(oldPixel);
			img->pixmap[x][y] = newPixel;
			Rgb quantError = oldPixel - newPixel;
			if (x < img->width - 1)
				img->pixmap[x + 1][y] = img->pixmap[x + 1][y] + 7. / 16 * quantError;
			if (x > 0 && y < img->height - 1)
				img->pixmap[x - 1][y + 1] = img->pixmap[x - 1][y + 1] + 3. / 16 * quantError;
			if (y < img->height - 1)
				img->pixmap[x][y + 1] = img->pixmap[x][y + 1] + 5. / 16 * quantError;
			if (x < img->width - 1 && y < img->height - 1)
				img->pixmap[x + 1][y + 1] = img->pixmap[x + 1][y + 1] + 1. / 16 * quantError;
		}
	}
}

Rgb ToGreyScalePixmapConverter::colorToYUV(Rgb pixel)
{
	int newColor = pixel.r*0.299 + pixel.g*0.587 + pixel.b*0.114;
	pixel.r = pixel.g = pixel.b = newColor;
	return pixel;
}


Rgb ToGreyScalePixmapConverter::findClosestPaletteColor(Rgb pixel)
{
	switch (pixel.r % 4)
	{
	case 0:
		break;
	case 1:
		pixel = pixel - Rgb { 1,1,1 };
		break;
	case 2:
		pixel = pixel - Rgb{ 2,2,2 };
		break;
	case 3:
		if (pixel.r == 255)
			pixel = Rgb{ 252, 252, 252 };
		else
			pixel = pixel + Rgb{ 1,1,1 };
		break;
	}
	return pixel;
}

