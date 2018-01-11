#pragma once
#include "types.h"

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

class XdFileWriter : public FileWriter
{
public:
	virtual ~XdFileWriter()=default;
	explicit XdFileWriter(const ImgWithParam* img): FileWriter(img){};
	bool write() override; //TODO: implement, zapisuje dane z img do pliku o ścieżce podanej w img
};

class BmpFileWriter : public FileWriter
{
public:
	virtual ~BmpFileWriter()=default;
	BmpFileWriter(const ImgWithParam* img): FileWriter(img){};
	bool write() override; //TODO: implement, zapisuje dane z img do pliku o ścieżce podanej w img
};

