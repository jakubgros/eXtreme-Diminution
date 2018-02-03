#pragma once
#include "FileWriter.h"

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
