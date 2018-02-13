#include "ToDedicatedPixmapConverter.h"
#include "Types.h"
#include "DitheringFS.h"
#include "RangeSort.h"
#include <iostream>


ToDedicatedPixmapConverter::ToDedicatedPixmapConverter(ImgWithParam* img) :
	BmpToXdConverter(img),
	pixmap_()
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
			pixmap_.push_back(img->pixmap[x][y]);
}

void ToDedicatedPixmapConverter::findPalette()
{
	createSortedPixmap();
	findNewColors();
}

void ToDedicatedPixmapConverter::createSortedPixmap()
{
	RangeSort rs(&img->pixmap, img->dedicatedColorPalette.size());
	pixmap_ = rs.sort();
}


void ToDedicatedPixmapConverter::findNewColors()
{
	if (pixmap_.size() <= img->dedicatedColorPalette.size())
		step_ = 1;
	else
		step_ = pixmap_.size() / img->dedicatedColorPalette.size();

	for (size_t i = 0 , numOfCols = 0; i < pixmap_.size(); i += step_, ++numOfCols)
	{
		if (numOfCols < img->dedicatedColorPalette.size())
		{
			long double sumL = 0;
			long double sumA = 0;
			long double sumB = 0;
			int j = i;
			for (j = i; j < i + step_; ++j)
			{
				Lab lab{};
				rgb2Lab(pixmap_[j].r, pixmap_[j].g, pixmap_[j].b, &lab.l, &lab.a, &lab.b);
				sumL += lab.l;
				sumA += lab.a;
				sumB += lab.b;
			}
			Lab avarage = { sumL / (step_), sumA / (step_), sumB / (step_) };
			Rgb newColor;
			lab2Rgb(avarage.l, avarage.a, avarage.b, &newColor.r, &newColor.g, &newColor.b);
			img->dedicatedColorPalette[numOfCols] = newColor;
		}
	}
}

void ToDedicatedPixmapConverter::rgb2Lab(int R, int G, int B, double *l, double *a, double *b)
{
	double var_R = R / 255.;
	double var_G = G / 255.;
	double var_B = B / 255.;

	if (var_R > 0.04045)
		var_R = pow(((var_R + 0.055) / 1.055), 2.4);
	else   
		var_R = var_R / 12.92;
	if (var_G > 0.04045)
		var_G = pow(((var_G + 0.055) / 1.055), 2.4);
	else
		var_G = var_G / 12.92;
	if (var_B > 0.04045)
		var_B = pow((var_B + 0.055) / 1.055, 2.4);
	else
		var_B = var_B / 12.92;

	var_R = var_R * 100;
	var_G = var_G * 100;
	var_B = var_B * 100;

	double X = var_R * 0.4124 + var_G * 0.3576 + var_B * 0.1805;
	double Y = var_R * 0.2126 + var_G * 0.7152 + var_B * 0.0722;
	double Z = var_R * 0.0193 + var_G * 0.1192 + var_B * 0.9505;

	double ReferenceX = 95.047;
	double ReferenceY = 100;
	double ReferenceZ = 108.883;

	double var_X = X / ReferenceX;
	double var_Y = Y / ReferenceY;
	double var_Z = Z / ReferenceZ;

	if (var_X > 0.008856)
		var_X = pow(var_X, (1. / 3));
	else
		var_X = (7.787 * var_X) + (16. / 116);
	if (var_Y > 0.008856)
		var_Y = pow(var_Y, (1. / 3));
	else
		var_Y = (7.787 * var_Y) + (16. / 116);
	if (var_Z > 0.008856)
		var_Z = pow(var_Z, (1. / 3));
	else
		var_Z = (7.787 * var_Z) + (16. / 116);

	*l = (116 * var_Y) - 16;
	*a = 500 * (var_X - var_Y);
	*b = 200 * (var_Y - var_Z);
}

void ToDedicatedPixmapConverter::lab2Rgb(double L, double A, double B, int *r, int *g, int *b)
{

	double ReferenceX = 95.047;
	double ReferenceY = 100;
	double ReferenceZ = 108.883;


	double var_Y = (L + 16) / 116.;
	double var_X = A / 500. + var_Y;
	double var_Z = var_Y - B / 200.;

	if (pow(var_Y, 3) > 0.008856)
		var_Y = pow(var_Y, 3);
	else
		var_Y = (var_Y - 16. / 116) / 7.787;
	if (pow(var_X, 3) > 0.008856)
		var_X = pow(var_X, 3);
	else
		var_X = (var_X - 16. / 116) / 7.787;
	if (pow(var_Z, 3) > 0.008856)
		var_Z = pow(var_Z, 3);
	else
		var_Z = (var_Z - 16. / 116) / 7.787;

	double X = var_X * ReferenceX;
	double Y = var_Y * ReferenceY;
	double Z = var_Z * ReferenceZ;

	var_X = X / 100;
	var_Y = Y / 100;
	var_Z = Z / 100;

	double var_R = var_X *  3.2406 + var_Y * -1.5372 + var_Z * -0.4986;
	double var_G = var_X * -0.9689 + var_Y *  1.8758 + var_Z *  0.0415;
	double var_B = var_X *  0.0557 + var_Y * -0.2040 + var_Z *  1.0570;

	if (var_R > 0.0031308)
		var_R = 1.055 * pow(var_R, (1 / 2.4)) - 0.055;
	else
		var_R = 12.92 * var_R;
	if (var_G > 0.0031308)
		var_G = 1.055 * pow(var_G, (1 / 2.4)) - 0.055;
	else
		var_G = 12.92 * var_G;
	if (var_B > 0.0031308)
		var_B = 1.055 * pow(var_B, (1 / 2.4)) - 0.055;
	else
		var_B = 12.92 * var_B;

	*r = var_R * 255;
	*g = var_G * 255;
	*b = var_B * 255;
}