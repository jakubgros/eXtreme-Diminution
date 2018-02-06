#include "XdFileReader.h"
#include <string>
#include <iostream>


void XdFileReader::read()
{
	readFileHeader();
	readImgHeader();
	readColorPalette();
	readDictionary();
	readPixmap();
}

XdFileReader::~XdFileReader()
{
	delete dictionary;
}

void XdFileReader::readFileType()
{
	img->fileType[0]=bitStream.readByte();
	img->fileType[1]=bitStream.readByte();
}

void XdFileReader::readFileSize()
{
	img->fileSize=bitStream.read<long long>(34);
}

void XdFileReader::readFileHeader()
{
	readFileType();
	readFileSize();
}

XdFileReader::XdFileReader(ImgWithParam* img): FileReader(img), dictionary(nullptr)
{
	bitStream.open(img->inputFilepath.c_str(), 'r');
}

void XdFileReader::readWidth()
{
	img->width=bitStream.read<int>(14);
}

void XdFileReader::readHeight()
{
	img->height=bitStream.read<int>(14);
}

void XdFileReader::readColorMode()
{
	int mode=bitStream.read<int>(2);
	std::cout << mode << std::endl;
	img->colorMode=ColorMode(mode);
}

void XdFileReader::readImgHeader()
{
	readWidth();
	readHeight();
	readColorMode();
}

void XdFileReader::readPixel(Rgb& p)
{
	const int BITS_IN_BYTE=8;
	p.r=bitStream.read<int>(sizeof(p.r)*BITS_IN_BYTE);
	p.g=bitStream.read<int>(sizeof(p.g)*BITS_IN_BYTE);
	p.b=bitStream.read<int>(sizeof(p.b)*BITS_IN_BYTE);
}

void XdFileReader::readColorPalette()
{
	if(img->colorMode==ColorMode::IMPOSED)
		return; //no need to write
	else if(img->colorMode==ColorMode::GREY_SCALE)
		return; //no need to write
	else if(img->colorMode==ColorMode::DEDICATED)
	{
		for(Rgb & p : img->dedicatedColorPalette)
			readPixel(p);
	}
}


//TODO: sprawdzic bitsety w readerze czy nie wczytuja sie od tylu albo zle, we writerach juz sa ponaprawiane
void XdFileReader::readCode(Word& word, size_t codeLength)
{
	std::bitset<64> codeWord=bitStream.readBitset(codeLength);
	std::string strCodeWord=codeWord.to_string();

	for(int i=codeWord.size()-codeLength; i<codeWord.size(); ++i)
		word.codeWord.push_back(strCodeWord[i]);
}

void XdFileReader::readWord(Word& word)
{
	size_t codeLength=bitStream.read<size_t>(6);
	readCode(word, codeLength);
	word.number=bitStream.read<int>(6);
}

void XdFileReader::readDictionary()
{
	int dictSize= bitStream.read<int>(6)+1;

	dictionary=new std::vector<Word>;
	dictionary->resize(dictSize);

	for(int i=0; i < dictSize; ++i)
		readWord((*dictionary)[i]);
}

bool XdFileReader::isCodeInDict(const std::string& str)
{
	for(int i=0; i<dictionary->size(); ++i)
	{
		if((*dictionary)[i].codeWord==str)
			return true;
	}
	return false;
}

int XdFileReader::getWordNumber(const std::string& code)
{
	for(int i=0; i<dictionary->size(); ++i)
	{
		if((*dictionary)[i].codeWord==code)
			return (*dictionary)[i].number;
	}

	throw std::exception("error in getWordNumber");
}

Rgb XdFileReader::getColorFromPalette(int wordNumber)
{
	Palette* palette;
	if(img->colorMode==ColorMode::GREY_SCALE)
		palette = &(img->imposedGreyPalette);
	else if(img->colorMode==ColorMode::DEDICATED)
		palette = &(img->dedicatedColorPalette);
	else if(img->colorMode==ColorMode::IMPOSED)
		palette = &(img->imposedColorPalette);
	else
		throw std::exception("error in getColorFromPalette");

	return (*palette)[wordNumber];
}



void XdFileReader::shiftIndexes(int& x, int& y)
{
	if(x!=0 && x%img->width==0)
	{
		x=0;
		++y;
	}
}

void XdFileReader::makeStepBack(std::string& code, char& lastBit)
{
	int lastPos=code.size()-1;
	lastBit = code[lastPos];
	code.erase(lastPos); 
}

void XdFileReader::initPixmap()
{
	img->pixmap.resize(img->width);
	for(int i=0; i<img->width; ++i)
		img->pixmap[i].resize(img->height);
}

void XdFileReader::readPixmap()
{
	initPixmap();

	std::string code;
	bool bit;
	long long pixmapSize=img->height*img->width;
	int counter = 0;
	while(counter < pixmapSize)
	{
		bitStream.readBit(bit); //wczytuje bit
		code+= bit?"1":"0"; //dodaje go do stringa

		if (isCodeInDict(code))
		{
			img->compressedPixmap.push_back(code);
			++counter;
			code = "";
		}	
	}
}



const Dictionary * XdFileReader::getDictionary()
{
	return dictionary;
}