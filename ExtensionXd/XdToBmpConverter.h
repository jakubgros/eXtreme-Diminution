#pragma once
#include "Converter.h"

class XdToBmpConverter : public Converter
{
public:
	explicit XdToBmpConverter(ImgWithParam* img, ColorMode mode);
	virtual ~XdToBmpConverter() = default;
	void convert();
private:
	ColorMode mode;
private:
	XdToBmpConverter(const XdToBmpConverter&);
	XdToBmpConverter& operator=(const XdToBmpConverter&);
};