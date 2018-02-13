#pragma once
#include "Converter.h"
#include <vector>
#include "types.h"

struct Rgb;

class ToDedicatedPixmapConverter : public BmpToXdConverter
{
public:
	explicit ToDedicatedPixmapConverter(ImgWithParam* imgWithParam);
	~ToDedicatedPixmapConverter() = default;
	void convert() override;
	void checkResources() const;
private:
	std::vector<Rgb> pixmap_;
	int step_;
	size_t numOfCols_;
private:
	static Lab rgb2Lab(const Rgb color);
	static Rgb lab2Rgb(const Lab color);
	void convertToOneDimensionPixmap();
	void findPalette();
	void sortPixmap();
	void findNewColors();
	void countStep();
	void addNewColor(const size_t startingPixel, const size_t endingPixel);
	Lab countLabAvarage(size_t begin, size_t end);
private:
	ToDedicatedPixmapConverter(const ToDedicatedPixmapConverter&);
	ToDedicatedPixmapConverter& operator=(const ToDedicatedPixmapConverter&);
};