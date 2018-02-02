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

class XdFileReader : public FileReader
{
protected:
	Dictionary* dictionary;
public:
	virtual ~XdFileReader();
	explicit XdFileReader(ImgWithParam* img): FileReader(img), dictionary(nullptr){};
	void read() override; 
	const Dictionary* getDictionary() override;
};

class BmpFileReader : public FileReader
{
public:
	virtual ~BmpFileReader()=default;
	explicit BmpFileReader(ImgWithParam* img): FileReader(img) {};
	void read() override; //TODO: wczytuje z pliku podanego jako input w img i zapisuje wszystko do img(tutaj nie ma slownika)
};