#pragma once
#include "Types.h"

class Decompressor
{
protected:
	ImgWithParam* img;
public:
	virtual ~Decompressor()=default;
	explicit Decompressor(ImgWithParam* img): img(img){};
	virtual void decompress(const Dictionary* dictionary)=0;
};

class BmpDecompressor : public Decompressor
{
public:
	virtual ~BmpDecompressor()=default;
	explicit BmpDecompressor(ImgWithParam* img): Decompressor(img){};
	void decompress(const Dictionary* dictionary=nullptr) override; //TODO: implement, dictionary parameter is not necessary, but added so that classes has the same interface 
};
