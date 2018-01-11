#pragma once
#include "types.h" //TODO: sprobowac sie pozbyc tego naglowka, wykorzystuje go tylko Dictionary

struct ImgWithParam;

class Compressor
{
protected:
	ImgWithParam* img;
public:
	virtual ~Compressor()=default;
	explicit Compressor(ImgWithParam* img): img(img){};
	virtual Dictionary* compress()=0;
};

class XdCompressor : public Compressor
{
	Dictionary* dictionary;
public:
	virtual ~XdCompressor();
	explicit XdCompressor(ImgWithParam* img): Compressor(img){};
	Dictionary* compress() override; //TODO: implement, zwraca słownik
};

class BmpCompressor : public Compressor
{
public:
	virtual ~BmpCompressor()=default;
	explicit BmpCompressor(ImgWithParam* img): Compressor(img){};
	Dictionary* compress() override; //TODO: implement, zwraca nullptr
};
