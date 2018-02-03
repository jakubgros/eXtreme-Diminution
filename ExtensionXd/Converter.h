#pragma once

struct ImgWithParam;

class Converter
{
protected:
	ImgWithParam* img;
public:
	virtual void convert()=0;
	explicit Converter(ImgWithParam* img): img(img){};
	virtual ~Converter()=default;
};

enum ColorMode;

class BmpToXdConverter: public Converter
{
public:
	virtual void convert()=0;
	explicit BmpToXdConverter(ImgWithParam* img): Converter(img){};
	virtual ~BmpToXdConverter()=default;
};