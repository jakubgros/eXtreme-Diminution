#pragma once
#include "types.h"
#include <string>
#include <iostream> //delete if no exception thrown

class PixmapToStringConverter
{
public:
	explicit PixmapToStringConverter(ImgWithParam* img);
	std::string convert();
	std::string getPixmap();
private:
	ImgWithParam* img;
	std::string pixmap;
private:
	void addColorFromPalette(const Rgb& pixel);
	void addFromGreyScale(const Rgb& pixel);
	void addFromImposed(const Rgb& pixel);
	void addFromDedicated(const Rgb& pixel);
};