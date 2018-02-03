#pragma once
#include "FileReader.h"
#include "FileBitStream.h"

class XdFileReader : public FileReader
{
	FileBitStream bitStream;
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
	void readColorPalette();
	void readCode(Word& word, size_t codeLength);
	void readWord(Word& word);
	void readDictionary();
	bool isCodeInDict(const std::string& str);
	int getWordNumber(const std::string& str);
	Rgb getColorFromPalette(int wordNumber);
	void addToPixmap(const std::string& code, int x, int y);
	void readPixmap();
	void read() override; 
	const Dictionary* getDictionary() override;
};
