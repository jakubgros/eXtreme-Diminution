#include "XdFileReader.h"
#include <string>

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
	img->colorMode=ColorMode(mode);
}

void XdFileReader::readImgHeader()
{
	readWidth();
	readHeight();
	readColorMode();
}

void XdFileReader::readColorPalette()
{
	if(img->colorMode==ColorMode::IMPOSED)
		return; //no need to write
	else if(img->colorMode==ColorMode::GREY_SCALE)
		return; //no need to write
	else if(img->colorMode==ColorMode::DEDICATED)
	{
		for(auto & p : img->dedicatedColorPalette)
		{
			p.r=bitStream.read<int>(sizeof(p.r));
			p.g=bitStream.read<int>(sizeof(p.g));
			p.b=bitStream.read<int>(sizeof(p.b));
		}
	}
}

void XdFileReader::readCode(Word& word, size_t codeLength)
{
	std::bitset<64> codeWord=bitStream.readBitset(codeLength);
	std::string strCodeWord=codeWord.to_string();
	for(int i=0; i<codeWord.size(); ++i)
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
	int dictSize= bitStream.read<int>(6);

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

int XdFileReader::getWordNumber(const std::string& str)
{
	for(int i=0; i<dictionary->size(); ++i)
	{
		if((*dictionary)[i].codeWord==str)
			return (*dictionary)[i].number;
	}

	throw std::exception("error in getWordNumber");
}

Rgb XdFileReader::getColorFromPalette(int wordNumber)
{
	Palette* palette;
	if(img->colorMode=ColorMode::GREY_SCALE)
		palette = &(img->imposedGreyPalette);
	else if(img->colorMode=ColorMode::DEDICATED)
		palette = &(img->dedicatedColorPalette);
	else if(img->colorMode=ColorMode::IMPOSED)
		palette = &(img->imposedColorPalette);
	else
		throw std::exception("error in getColorFromPalette");

	return (*palette)[wordNumber];
}

void XdFileReader::addToPixmap(const std::string& code, int x, int y)
{
	int wordNumber=getWordNumber(code); //pobieram numer koloru ze slownika
	Rgb pixel=getColorFromPalette(wordNumber);
	img->pixmap[x][y] = pixel;
}

void XdFileReader::readPixmap()
{
	img->pixmap.resize(img->width);
	for(int i=0; i<img->width; ++i)
		img->pixmap[i].resize(img->height);

	int x=0;
	int y=0;
	std::string code;
	bool bit;
	long long pixmapSize=img->height*img->width;
	for(int i=0; i<pixmapSize; ++i)
	{
		bitStream.readBit(bit); //wczytuje bit
		code+= bit?"1":"0"; //dodaje go do stringa

		if(isCodeInDict(code)) //jak jest w slowniku
			continue;
		else // jak nie ma to cofam ostatni krok (ostatnio dodany bit) i dodaje do pixmapy
		{
			if(x!=0 && x%img->width==0)
			{
				x=0;
				++y;
			}
			int lastPos=code.size()-1; //pozycja ostatnio dodanego bitu
			char lastBit=code[lastPos]; //zapisuje ostatni bit
			code.erase(lastPos); //usuwam go

			addToPixmap(code, x, y); // dodaje do pixmapy
			code=lastBit; //ustawiam code na ostatnio wczytany bit(bo on juz nie nalezal do tego 

			++x;
		}
			
	}
}

void XdFileReader::read()
{
	readFileHeader();
	readImgHeader();
	readColorPalette();
	readDictionary();
	readPixmap();
}

const Dictionary * XdFileReader::getDictionary()
{
	return dictionary;
}

