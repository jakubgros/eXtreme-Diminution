#pragma once
#include "types.h"


class Decompressor
{
protected:
	ImgWithParam* img;
public:
	virtual ~Decompressor()=default;
	explicit Decompressor(ImgWithParam* img): img(img){};
	virtual void decompress(const Dictionary* dictionary)=0;
};

class XdDecompressor : public Decompressor
{
public:
	virtual ~XdDecompressor()=default;
	explicit XdDecompressor(ImgWithParam* img): Decompressor(img){};
	void decompress(const Dictionary* dictionary) override; //TODO: implement
};

class BmpDecompressor : public Decompressor
{
public:
	virtual ~BmpDecompressor()=default;
	explicit BmpDecompressor(ImgWithParam* img): Decompressor(img){};
	void decompress(const Dictionary* dictionary=nullptr) override; //TODO: implement, dictionary parameter is not necessary, but added so that classes has the same interface 
};
