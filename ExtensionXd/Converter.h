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

class XdToBmpConverter: public Converter
{
	ColorMode mode;
public:
	virtual void convert(); //TODO: implement
	explicit XdToBmpConverter(ImgWithParam* img, ColorMode mode): Converter(img), mode(mode){};
	virtual ~XdToBmpConverter()=default;
};

class BmpToXdConverter: public Converter
{
public:
	virtual void convert()=0;
	explicit BmpToXdConverter(ImgWithParam* img): Converter(img){};
	virtual ~BmpToXdConverter()=default;
};

class ToImposedPixmapConverter : public BmpToXdConverter
{
public:
	void convert() override; //TODO: implement
	explicit ToImposedPixmapConverter(ImgWithParam* img): BmpToXdConverter(img){};
	virtual ~ToImposedPixmapConverter()=default;
};

class ToGreyScalePixmapConverter : public BmpToXdConverter
{
public:
	virtual void convert(); //TODO: implement
	explicit ToGreyScalePixmapConverter(ImgWithParam* img): BmpToXdConverter(img){}
	virtual ~ToGreyScalePixmapConverter()=default;
};

class ToDedicatedPixmapConverter : public BmpToXdConverter
{
public:
	virtual void convert(); //TODO: implement
	explicit ToDedicatedPixmapConverter(ImgWithParam* img): BmpToXdConverter(img){};
	virtual ~ToDedicatedPixmapConverter()=default;
};
