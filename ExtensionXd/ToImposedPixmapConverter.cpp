#include "ToImposedPixmapConverter.h"
#include "Types.h"

ToImposedPixmapConverter::ToImposedPixmapConverter(ImgWithParam* img) :
	BmpToXdConverter(img)
{

}

void ToImposedPixmapConverter::convert()
{
	for (int y = 0; y < img->height; ++y)
	{
		for (int x = 0; x < img->width; ++x)
		{
			Rgb oldPixel = img->pixmap[x][y];
			Rgb newPixel = findClosestPaletteColor(oldPixel);
			img->pixmap[x][y] = newPixel;
			Rgb quantError = oldPixel - newPixel;
			if (x < img->width - 1)
				img->pixmap[x + 1][y    ] = img->pixmap[x + 1][y    ] + 7. / 16 * quantError;
			if (x > 0 && y < img->height - 1)
				img->pixmap[x - 1][y + 1] = img->pixmap[x - 1][y + 1] + 3. / 16 * quantError;
			if(y < img->height - 1)
				img->pixmap[x    ][y + 1] = img->pixmap[x    ][y + 1] + 5. / 16 * quantError;
			if (x < img->width - 1 && y < img->height - 1)
				img->pixmap[x + 1][y + 1] = img->pixmap[x + 1][y + 1] + 1. / 16 * quantError;
		}
	}
}


double ToImposedPixmapConverter::countDistance(Rgb pixel1, Rgb pixel2)
{
	return pow((pixel1.r - pixel2.r)*0.299, 2)
		+ pow((pixel1.g - pixel2.g)*0.587, 2)
		+ pow((pixel1.b - pixel2.b)*0.114, 2); 
}


Rgb ToImposedPixmapConverter::findClosestPaletteColor(Rgb pixel)
{
	double bestDistance = countDistance(pixel, img->imposedColorPalette[0]);
	int bestColor = 0;
	double currentDistance = 0;
	for (int i = 1; i < img->imposedColorPalette.size(); ++i)
	{
		currentDistance = countDistance(pixel, img->imposedColorPalette[i]);
		if (currentDistance < bestDistance)
			bestColor = i;
	}
	return  img->imposedColorPalette[bestColor];
}