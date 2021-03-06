﻿#pragma once
#include "FileReader.h"
#include "FileBitStream.h"
#include <unordered_map>
class XdFileReader : public FileReader
{
	FileBitStream bitStream;
	std::unordered_map<std::string, int> hashedDictionary;
protected:
	Dictionary* dictionary;
public:
	virtual ~XdFileReader();
	void readFileType();
	void readFileSize();
	void readFileHeader();
	explicit XdFileReader(ImgWithParam* img);
	void readWidth();
	void readHeight();
	void readColorMode();
	void readImgHeader();
	void readPixel(Rgb& p);
	void readColorPalette();
	void readCode(Word& word, size_t codeLength);
	void readWord(Word& word);
	void makeHashedDict();
	void readDictionary();
	bool isCodeInDict(const std::string& str);
	int getWordNumber(const std::string& str);
	Rgb getColorFromPalette(int wordNumber);
	void shiftIndexes(int& x, int& y);
	void makeStepBack(std::string& code, char& lastBit);
	void initPixmap();
	void readPixmap();
	void read() override; 
	const Dictionary* getDictionary() override;
};
