#pragma once
#include "Types.h"
#include "FileBitStream.h"
#include <bitset>

class FileWriter
{
protected:
	const ImgWithParam* img;
	Dictionary* dictionary;

public:
	virtual ~FileWriter()=default;
	explicit FileWriter(const ImgWithParam* img): img(img), dictionary(nullptr){};
	virtual bool write()=0; //returns true if success, false if not (e.g. there is already a file with the same name)
	void setDictionaryToSave(Dictionary* dict);
};

