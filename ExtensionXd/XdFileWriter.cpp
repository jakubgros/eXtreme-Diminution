#include "XdFileWriter.h"
#include "FileBitStream.h"
#include <bitset>
#include <iostream>



XdFileWriter::XdFileWriter(const ImgWithParam* img): FileWriter(img)
{
	bitStream.open(img->outputFilepath.c_str(), 'w');
}

bool XdFileWriter::write()
{
	writeFileHeader();
	writeImgHeader();
	wirteColorPalette();
	writeDictionary();
	writePixmap();
	bitStream.flush();
	
	return true; //TODO: reimplement function so as to it returned false if it wants to overwrite a file
}

void XdFileWriter::writeFileHeader()
{
	writeFileType();
	writeFileSize();
}

void XdFileWriter::writeFileType()
{
	bitStream.write('x');
	bitStream.write('d');
}

void XdFileWriter::writeFileSize() //TODO: implement
{
	for(int i=0; i<34; ++i)
		bitStream.writeBit(0);
}

void XdFileWriter::writeImgHeader()
{
	bitStream.write(img->width, 14);
	bitStream.write(img->height, 14);
	bitStream.write(img->colorMode, 2);
}

void XdFileWriter::wirteColorPalette()
{
	Palette palette;
	if(img->colorMode==ColorMode::IMPOSED)
		return; //no need to write
	else if(img->colorMode==ColorMode::GREY_SCALE)
		return; //no need to write
	else if(img->colorMode==ColorMode::DEDICATED)
		palette = img->dedicatedColorPalette;
	


	for(int i=0; i<palette.size(); ++i)
	{
		bitStream.write(palette[i].r, sizeof(palette[i].r));
		bitStream.write(palette[i].g, sizeof(palette[i].g));
		bitStream.write(palette[i].b, sizeof(palette[i].b));
	}
		
}

void XdFileWriter::writeDictionary()
{
	writeDictSize();
	writeDictWords();
}

void XdFileWriter::writeDictSize()
{
	size_t size=dictionary->size();
	bitStream.write(size, 6);
}

void XdFileWriter::writeDictWords()
{
	for(int i=0; i < dictionary->size(); ++i)
		writeWord((*dictionary)[i]);
}

void XdFileWriter::writeWord(const Word& word)
{
	//TODO: jakby nie dzia³a³o operator[] w bitsetach poni¿szych 3 metod to trzeba zmienic na metode test(i)
	writeWordsLength(word); 
	writeCodeWord(word);
	writePaletteIndex(word);
}

void XdFileWriter::writeWordsLength(const Word& word)
{
	int a=word.codeWord.length(); //TODO usunac
	std::bitset<6> length(word.codeWord.length());
	for(int i=0; i<length.size(); ++i)
		bitStream.writeBit(length[i]);
}

void XdFileWriter::writeCodeWord(const Word& word)
{
	std::bitset<63> codeWord(word.codeWord);

	size_t mostSignificantDigitPos=findMostSignificantDigit(codeWord);
	for(int i=0; i<=mostSignificantDigitPos; ++i)
		bitStream.writeBit(codeWord[i]);
}

void XdFileWriter::writePaletteIndex(const Word& word)
{
	std::bitset<6> number(word.number);
	for(int i=0; i<number.size(); ++i)
		bitStream.writeBit(number[i]);
}

void XdFileWriter::writePixmap()
{
	int a=img->compressedPixmap.size(); //TODO usunac
	for(int i=0; i<img->compressedPixmap.size(); ++i)
	{
		std::string strCode=img->compressedPixmap[i];
		std::bitset<63> bitCode(strCode);
		size_t mostSignifDigPos=findMostSignificantDigit(bitCode);
		writeSinglePixel(bitCode, mostSignifDigPos);
	}
}

size_t XdFileWriter::findMostSignificantDigit(std::bitset<63>& bitCode)
{
	size_t mostSignificantDigitPos = 0;
	for(int i=bitCode.size()-1; i>=0; --i)
	{
		if(bitCode.test(i)==true)
		{
			mostSignificantDigitPos=i;
			break;
		}
	}

	return mostSignificantDigitPos;
}

void XdFileWriter::writeSinglePixel(std::bitset<63> bitCode, size_t mostSignificantDigitPos)
{
	for(int i=0; i<=mostSignificantDigitPos; ++i)
		bitStream.writeBit(bitCode[i]);
}