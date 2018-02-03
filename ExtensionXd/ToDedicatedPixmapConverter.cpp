#include "ToDedicatedPixmapConverter.h"
#include "Types.h"
#include <iostream>

ToDedicatedPixmapConverter::ToDedicatedPixmapConverter(ImgWithParam* img) :
	BmpToXdConverter(img)
{

}

bool compareR(Rgb a, Rgb b)
{
	return a.r < b.r;
}
bool compareG(Rgb a, Rgb b)
{
	return a.g < b.g;
}
bool compareB(Rgb a, Rgb b)
{
	return a.b < b.b;
}

void ToDedicatedPixmapConverter::convert()
{
	createPixmap();
	findPalette();
	dithering();
}

void  ToDedicatedPixmapConverter::createPixmap()
{
	for (int y = 0; y < img->height; ++y)
		for (int x = 0; x < img->width; ++x)
			pixmap.push_back(img->pixmap[x][y]);
}

void ToDedicatedPixmapConverter::findPalette()
{
	sortBuckets(0, pixmap.size() - 1);
	findNewColors();
}

void ToDedicatedPixmapConverter::sortBuckets(const int left,const int right)
{
	findRanges(left, right);
	if (rRange == std::max(rRange, std::max(bRange, gRange)))
		std::sort(pixmap.begin() + left, pixmap.begin() + right + 1, compareR);
	else
		if (gRange == std::max(gRange, bRange))
			std::sort(pixmap.begin() + left, pixmap.begin() + right + 1, compareG);
		else
			std::sort(pixmap.begin() + left, pixmap.begin() + right + 1, compareB);
	if (!isLastPartition(left, right))
		sortBuckets(left, left + (right - left) / 2);
	if (!isLastPartition(left, right))
		sortBuckets(left + (right - left) / 2 + 1, right);
}

void ToDedicatedPixmapConverter::findRanges(const int left, const int right)
{
	initRanges(left);
	for (int i = left; i <= right; ++i)
		updateRanges(i);
}


bool ToDedicatedPixmapConverter::isLastPartition(const int left, const int right)
{
	return (right - left <= img->height*img->width / img->dedicatedColorPalette.size());
}

void ToDedicatedPixmapConverter::initRanges(const int index)
{
	rRange.setMin(pixmap[index].r);
	rRange.setMax(pixmap[index].r);
	gRange.setMin(pixmap[index].g);
	gRange.setMax(pixmap[index].g);
	bRange.setMin(pixmap[index].b);
	bRange.setMax(pixmap[index].b);
}

void ToDedicatedPixmapConverter::updateRanges(const int index)
{
	rRange.update(pixmap[index].r);
	gRange.update(pixmap[index].g);
	bRange.update(pixmap[index].b);
}

void ToDedicatedPixmapConverter::findNewColors()
{
	int step = pixmap.size() / img->dedicatedColorPalette.size();
	int numOfCols = 0;
	for (int i = 0; i < pixmap.size(); i += step)
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
				RGB2LAB(pixmap[j].r, pixmap[j].g, pixmap[j].b, &lab.l, &lab.a, &lab.b);
				sumL += lab.l;
				sumA += lab.a;
				sumB += lab.b;
			}
			Lab avarage = { sumL / (step), sumA / (step), sumB / (step) };
			Rgb newColor;
			LAB2RGB(avarage.l, avarage.a, avarage.b, &newColor.r, &newColor.g, &newColor.b);
			img->dedicatedColorPalette[numOfCols] = newColor;
			++numOfCols;
		}
	}
}

void ToDedicatedPixmapConverter::RGB2LAB(int R, int G, int B, double *l, double *a, double *b) {
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

void ToDedicatedPixmapConverter::LAB2RGB(double L, double A, double B, int *r, int *g, int *b) {
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

void ToDedicatedPixmapConverter::dithering()
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

Rgb ToDedicatedPixmapConverter::findClosestPaletteColor(Rgb pixel)
{
	double bestDistance = countDistance(pixel, img->dedicatedColorPalette[0]);
	int bestColor = 0;
	double currentDistance = 0;
	for (int i = 1; i < img->dedicatedColorPalette.size(); ++i)
	{
		currentDistance = countDistance(pixel, img->dedicatedColorPalette[i]);
		if (currentDistance < bestDistance)
			bestColor = i;
	}
	return  img->dedicatedColorPalette[bestColor];
}

double ToDedicatedPixmapConverter::countDistance(Rgb pixel1, Rgb pixel2)
{
	return pow((pixel1.r - pixel2.r)*0.299, 2)
		+ pow((pixel1.g - pixel2.g)*0.587, 2)
		+ pow((pixel1.b - pixel2.b)*0.114, 2);
}