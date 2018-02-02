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

class XdFileWriter : public FileWriter
{
	FileBitStream bitStream;

	void writeFileType();
	void writeFileSize();
	void writeFileHeader();
	void writeImgHeader();
	void writeWordsLength(const Word& word);
	void writeCodeWord(const Word& word);
	void writePaletteIndex(const Word& word);
	void writeWord(const Word& word);
	size_t XdFileWriter::findMostSignificantDigit(std::bitset<63>& bitCode);
	void writeSinglePixel(std::bitset<63> bitCode, size_t mostSignificantDigitPos);
	void wirteColorPalette();
	void writeDictSize();
	void writeDictWords();
	void writeDictionary();
	void writePixmap();

public:
	virtual ~XdFileWriter()=default;
	explicit XdFileWriter(const ImgWithParam* img);
	bool write() override; 
};

class BmpFileWriter : public FileWriter
{
public:
	virtual ~BmpFileWriter()=default;
	BmpFileWriter(const ImgWithParam* img): FileWriter(img){};
	bool write() override; 
};

