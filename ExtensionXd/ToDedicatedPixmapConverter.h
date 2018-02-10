#pragma once
#include "Converter.h"
#include <vector>
#include <queue>
#include "Range.h"
#include <iostream>

struct Rgb;

class ToDedicatedPixmapConverter : public BmpToXdConverter
{
public:
	explicit ToDedicatedPixmapConverter(ImgWithParam* imgWithParam);
	~ToDedicatedPixmapConverter() = default;
	void convert();
private:
	std::vector<Rgb> pixmap;
private:
	void createPixmap();
	void createSortedPixmap();
	void findPalette();
	void findNewColors();
	static void rgb2Lab(int R, int G, int B, double *l, double *a, double *b);
	static void lab2Rgb(double L, double A, double B, int *r, int *g, int *b);
private:
	ToDedicatedPixmapConverter(const ToDedicatedPixmapConverter&);
	ToDedicatedPixmapConverter& operator=(const ToDedicatedPixmapConverter&);
};