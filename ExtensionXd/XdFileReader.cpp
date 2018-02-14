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

void XdFileReader::readFileHeader()
{
	readFileType();
	readFileSize();
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

void XdFileReader::readImgHeader()
{
	readWidth();
	readHeight();
	readColorMode();
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
	img->colorMode=ColorMode(mode);
}

void XdFileReader::readColorPalette()
{
	if(img->colorMode==ColorMode::IMPOSED)
		return; //no need to read
	else if(img->colorMode==ColorMode::GREY_SCALE)
		return; //no need to read
	else if(img->colorMode==ColorMode::DEDICATED)
	{
		for(Rgb & p : img->dedicatedColorPalette)
			readPixel(p);
	}
}

void XdFileReader::readPixel(Rgb& p)
{
	const int BITS_IN_BYTE=8;
	p.r=bitStream.read<int>(sizeof(p.r)*BITS_IN_BYTE);
	p.g=bitStream.read<int>(sizeof(p.g)*BITS_IN_BYTE);
	p.b=bitStream.read<int>(sizeof(p.b)*BITS_IN_BYTE);
}

void XdFileReader::readDictionary()
{
	int dictSize= bitStream.read<int>(6)+1;

	dictionary=new std::vector<Word>;
	dictionary->resize(dictSize);

	for(int i=0; i < dictSize; ++i)
		readWord((*dictionary)[i]);

	makeHashedDict();
}

void XdFileReader::readWord(Word& word)
{
	size_t codeLength=bitStream.read<size_t>(6);
	readCode(word, codeLength);
	word.number=bitStream.read<int>(6);
}

void XdFileReader::makeHashedDict()
{
	for(auto iter=dictionary->begin(); iter!=dictionary->end(); ++iter)
		hashedDictionary.insert(std::pair<std::string, int>(iter->codeWord, iter->number));	
}

void XdFileReader::readCode(Word& word, size_t codeLength)
{
	std::bitset<64> codeWord=bitStream.readBitset(codeLength);
	std::string strCodeWord=codeWord.to_string();

	for(int i=64 - codeLength; i<64; ++i)
		word.codeWord.push_back(strCodeWord[i]);
}

void XdFileReader::readPixmap()
{
	initPixmap();

	std::string code;
	bool bit;
	long long pixmapSize=img->height*img->width;
	int added = 0;
	while(added < pixmapSize)
	{
		if(bitStream.readBit(bit)==false)
			throw std::exception("Can't read bit. There is no more data in stream.");

		code+= bit?"1":"0";

		if (isCodeInDict(code))
		{
			img->compressedPixmap.push_back(code);
			++added;
			code = "";
		}	
	}
}

void XdFileReader::initPixmap()
{
	img->pixmap.resize(img->width);
	for(int i=0; i<img->width; ++i)
		img->pixmap[i].resize(img->height);
}

bool XdFileReader::isCodeInDict(const std::string& str)
{
	return hashedDictionary.find(str)!=hashedDictionary.end();
}

const Dictionary * XdFileReader::getDictionary()
{
	return dictionary;
}

XdFileReader::~XdFileReader()
{
	delete dictionary;
}

XdFileReader::XdFileReader(ImgWithParam* img): FileReader(img), dictionary(nullptr)
{
	bitStream.open(img->inputFilepath.c_str(), 'r');
}

