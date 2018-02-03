#pragma once
#include "Types.h"

class Validator
{
protected:
	ImgWithParam* img;
public:
	virtual ~Validator()=default;
	explicit Validator(ImgWithParam* img): img(img){};
	virtual bool validate()=0;
};

class BmpValidator : public Validator
{
public:
	virtual ~BmpValidator()=default;
	explicit BmpValidator(ImgWithParam* img): Validator(img){};
	bool validate() override {return true;}; //TODO
};

class XdValidator : public Validator
{
public:
	virtual ~XdValidator()=default;
	explicit XdValidator(ImgWithParam* img): Validator(img){};
	bool validate() override {return true;}; //TODO
};
