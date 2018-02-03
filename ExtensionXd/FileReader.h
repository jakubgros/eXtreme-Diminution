#pragma once
#include "Types.h"

class FileReader
{
protected:
	ImgWithParam* img;
public:
	FileReader(ImgWithParam* img): img(img){};
	virtual ~FileReader()=default;
	virtual void read()=0;
	virtual const Dictionary* getDictionary() { return nullptr;}
};

