#include "ToDedicatedPixmapConverter.h"
#include "types.h"
#include "DitheringFS.h"
#include "RangeSort.h"


ToDedicatedPixmapConverter::ToDedicatedPixmapConverter(ImgWithParam* img) :
	BmpToXdConverter(img),
	pixmap_(),
	step_(0),
	numOfCols_(0)
{

}

void ToDedicatedPixmapConverter::convert()
{
	checkResources();
	convertToOneDimensionPixmap();
	findPalette();
	dfs::ditheringFs(&img->pixmap, &img->dedicatedColorPalette);
}

void ToDedicatedPixmapConverter::checkResources() const
{
	if (img == nullptr)
		throw std::logic_error("ToDedidactedPixmapConverter - nullptr resources");
}

void  ToDedicatedPixmapConverter::convertToOneDimensionPixmap()
{
	for (size_t y = 0; y < img->height; ++y)
		for (size_t x = 0; x < img->width; ++x)
			pixmap_.push_back(img->pixmap[x][y]);
}

void ToDedicatedPixmapConverter::findPalette()
{
	sortPixmap();
	findNewColors();
}

void ToDedicatedPixmapConverter::sortPixmap()
{
	RangeSort rs(&img->pixmap, img->dedicatedColorPalette.size());
	pixmap_ = rs.sort();
}

void ToDedicatedPixmapConverter::findNewColors()
{
	countStep();
	for (size_t startingPixel = 0; startingPixel < pixmap_.size(); startingPixel += step_)
		if (numOfCols_ < img->dedicatedColorPalette.size())
			addNewColor(startingPixel, startingPixel + step_);
		else
			break;
}

void ToDedicatedPixmapConverter::countStep()
{
	if (pixmap_.size() <= img->dedicatedColorPalette.size())
		step_ = 1;
	else
		step_ = pixmap_.size() / img->dedicatedColorPalette.size();
}

void ToDedicatedPixmapConverter::addNewColor(const size_t startingPixel, const size_t endingPixel)
{
	const Lab avarage = countLabAvarage(startingPixel, endingPixel);
	const Rgb newColor = lab2Rgb(avarage);
	img->dedicatedColorPalette[numOfCols_] = newColor;
	++numOfCols_;
}

Lab ToDedicatedPixmapConverter::countLabAvarage(size_t begin, size_t end)
{
	long double sumL = 0;
	long double sumA = 0;
	long double sumB = 0;
	for (size_t j = begin; j < end; ++j)
	{
		const Lab lab = rgb2Lab(pixmap_[j]);
		sumL += lab.l;
		sumA += lab.a;
		sumB += lab.b;
	}
	const size_t numOfItems = end - begin;
	return { static_cast<double>(sumL / numOfItems), static_cast<double>(sumA / numOfItems), static_cast<double>(sumB / numOfItems) };
}

Lab ToDedicatedPixmapConverter::rgb2Lab(const Rgb color)
{
	double var_R = color.r / 255.;
	double var_G = color.g / 255.;
	double var_B = color.b / 255.;

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

	const double ReferenceX = 95.047;
	const double ReferenceY = 100;
	const double ReferenceZ = 108.883;

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

	Lab newColor = {};
	newColor.l = (116 * var_Y) - 16;
	newColor.a = 500 * (var_X - var_Y);
	newColor.b = 200 * (var_Y - var_Z);
	return newColor;
}

Rgb ToDedicatedPixmapConverter::lab2Rgb(const Lab color)
{
	const double ReferenceX = 95.047;
	const double ReferenceY = 100;
	const double ReferenceZ = 108.883;

	double var_Y = (color.l + 16) / 116.;
	double var_X = color.a / 500. + var_Y;
	double var_Z = var_Y - color.b / 200.;

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

	Rgb newColor{};
	newColor.r = var_R * 255;
	newColor.g = var_G * 255;
	newColor.b = var_B * 255;
	return newColor;
}