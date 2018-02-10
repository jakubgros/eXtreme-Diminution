#include "ToDedicatedPixmapConverter.h"
#include "Types.h"
#include "DitheringFS.h"
#include "RangeSort.h"
#include <iostream>

ToDedicatedPixmapConverter::ToDedicatedPixmapConverter(ImgWithParam* img) :
	BmpToXdConverter(img),
	pixmap()
{

}

void ToDedicatedPixmapConverter::convert()
{
	createPixmap();
	findPalette();
	dfs::ditheringFs(&img->pixmap, &img->dedicatedColorPalette);
}

void  ToDedicatedPixmapConverter::createPixmap()
{
	for (size_t y = 0; y < img->height; ++y)
		for (size_t x = 0; x < img->width; ++x)
			pixmap.push_back(img->pixmap[x][y]);
}

void ToDedicatedPixmapConverter::findPalette()
{
	createSortedPixmap();
	findNewColors();
}

void ToDedicatedPixmapConverter::createSortedPixmap()
{
	RangeSort rs(&img->pixmap, img->dedicatedColorPalette.size());
	pixmap = rs.sort();
}


void ToDedicatedPixmapConverter::findNewColors()
{
	const int step = pixmap.size() / img->dedicatedColorPalette.size();
	unsigned numOfCols = 0;
	//	for (size_t i = 0; i < pixmap.size(); ++i)
	//		std::cout << i << "| " << i%step << " | R: " << pixmap[i].r << " G:" << pixmap[i].g << " B:" << pixmap[i].b << std::endl;
	for (size_t i = 0; i < pixmap.size(); i += step)
	{
		if (numOfCols < img->dedicatedColorPalette.size())
		{
			long double sumL = 0;
			long double sumA = 0;
			long double sumB = 0;
			int j = i;
			for (j = i; j < i + step; ++j)
			{
				Lab lab;
				rgb2Lab(pixmap[j].r, pixmap[j].g, pixmap[j].b, &lab.l, &lab.a, &lab.b);
				sumL += lab.l;
				sumA += lab.a;
				sumB += lab.b;
			}
			Lab avarage = { sumL / (step), sumA / (step), sumB / (step) };
			Rgb newColor;
			lab2Rgb(avarage.l, avarage.a, avarage.b, &newColor.r, &newColor.g, &newColor.b);
			img->dedicatedColorPalette[numOfCols] = newColor;
			++numOfCols;
			//			std::cout << "< " << i << " , " << j-1 << " >" << std::endl;
		}
	}
}

void ToDedicatedPixmapConverter::rgb2Lab(int R, int G, int B, double *l, double *a, double *b)
{
	float RGB[3], XYZ[3];

	RGB[0] = R * 0.003922;
	RGB[1] = G * 0.003922;
	RGB[2] = B * 0.003922;

	RGB[0] = (RGB[0] > 0.04045) ? pow(((RGB[0] + 0.055) / 1.055), 2.4) : RGB[0] / 12.92;
	RGB[1] = (RGB[1] > 0.04045) ? pow(((RGB[1] + 0.055) / 1.055), 2.4) : RGB[1] / 12.92;
	RGB[2] = (RGB[2] > 0.04045) ? pow(((RGB[2] + 0.055) / 1.055), 2.4) : RGB[2] / 12.92;

	XYZ[0] = 0.412424  * RGB[0] + 0.357579 * RGB[1] + 0.180464  * RGB[2];
	XYZ[1] = 0.212656  * RGB[0] + 0.715158 * RGB[1] + 0.0721856 * RGB[2];
	XYZ[2] = 0.0193324 * RGB[0] + 0.119193 * RGB[1] + 0.950444  * RGB[2];

	*l = 116 * ((XYZ[1] / 1.000000) > 0.008856 ? pow(XYZ[1] / 1.000000, 0.333333) : 7.787 * XYZ[1] / 1.000000 + 0.137931) - 16;
	*a = 500 * (((XYZ[0] / 0.950467) > 0.008856 ? pow(XYZ[0] / 0.950467, 0.333333) : 7.787 * XYZ[0] / 0.950467 + 0.137931) - ((XYZ[1] / 1.000000) > 0.008856 ? pow(XYZ[1] / 1.000000, 0.333333) : 7.787 * XYZ[1] / 1.000000 + 0.137931));
	*b = 200 * (((XYZ[1] / 1.000000) > 0.008856 ? pow(XYZ[1] / 1.000000, 0.333333) : 7.787 * XYZ[1] / 1.000000 + 0.137931) - ((XYZ[2] / 1.088969) > 0.008856 ? pow(XYZ[2] / 1.088969, 0.333333) : 7.787 * XYZ[2] / 1.088969 + 0.137931));
}

void ToDedicatedPixmapConverter::lab2Rgb(double L, double A, double B, int *r, int *g, int *b)
{
	float XYZ[3], RGB[3];

	XYZ[1] = (L + 16) / 116;
	XYZ[0] = A / 500 + XYZ[1];
	XYZ[2] = XYZ[1] - B / 200;

	XYZ[1] = (XYZ[1] * XYZ[1] * XYZ[1] > 0.008856) ? XYZ[1] * XYZ[1] * XYZ[1] : (XYZ[1] - (16 / 116)) / 7.787;
	XYZ[0] = (XYZ[0] * XYZ[0] * XYZ[0] > 0.008856) ? XYZ[0] * XYZ[0] * XYZ[0] : (XYZ[0] - (16 / 116)) / 7.787;
	XYZ[2] = (XYZ[2] * XYZ[2] * XYZ[2] > 0.008856) ? XYZ[2] * XYZ[2] * XYZ[2] : (XYZ[2] - (16 / 116)) / 7.787;

	RGB[0] = 0.950467 * XYZ[0] * 3.2406 + 1.000000 * XYZ[1] * -1.5372 + 1.088969 * XYZ[2] * -0.4986;
	RGB[1] = 0.950467 * XYZ[0] * -0.9689 + 1.000000 * XYZ[1] * 1.8758 + 1.088969 * XYZ[2] * 0.0415;
	RGB[2] = 0.950467 * XYZ[0] * 0.0557 + 1.000000 * XYZ[1] * -0.2040 + 1.088969 * XYZ[2] * 1.0570;

	*r = (255 * ((RGB[0] > 0.0031308) ? 1.055 * (pow(RGB[0], (1 / 2.4)) - 0.055) : RGB[0] * 12.92));
	*g = (255 * ((RGB[1] > 0.0031308) ? 1.055 * (pow(RGB[1], (1 / 2.4)) - 0.055) : RGB[1] * 12.92));
	*b = (255 * ((RGB[2] > 0.0031308) ? 1.055 * (pow(RGB[2], (1 / 2.4)) - 0.055) : RGB[2] * 12.92));
}