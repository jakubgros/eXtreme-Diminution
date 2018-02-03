#pragma once
#include "Converter.h"
#include <vector>
#include "Range.h"

struct Rgb;

class ToDedicatedPixmapConverter : public BmpToXdConverter
{
public:
	explicit ToDedicatedPixmapConverter(ImgWithParam* img);
	virtual ~ToDedicatedPixmapConverter() = default;
	virtual void convert() override;
private:
	std::vector<Rgb> pixmap;
	Range rRange;
	Range bRange;
	Range gRange;
private:
	void createPixmap();
	void findPalette();
	void sortBuckets(const int left,const int right);
	void findNewColors();
	void dithering();
	void initRanges(const int init);
	void updateRanges(const int index);
	void findRanges(const int left, const int right);
	bool isLastPartition(const int left,const int right);
	double countDistance(Rgb pixel1, Rgb pixel2);
	Rgb findClosestPaletteColor(Rgb pixel);
	void RGB2LAB(int R, int G, int B, double *l, double *a, double *b);
	void LAB2RGB(double L, double A, double B, int *r, int *g, int *b);
private:
	ToDedicatedPixmapConverter(const ToDedicatedPixmapConverter&);
	ToDedicatedPixmapConverter& operator=(const ToDedicatedPixmapConverter&);
};