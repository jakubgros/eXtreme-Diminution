#include "FileWriter.h"
#include "FileBitStream.h"
#include <bitset>

void FileWriter::setDictionaryToSave(Dictionary* dict)
{
	dictionary=dict;
}

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
	
	return true; //TODO: reimplement function so as to it returned false if it wants to overwrite a file
}

void XdFileWriter::writeFileHeader()
{
	writeFileType();
	writeFileSize();
}

void XdFileWriter::writeFileType()
{
	bitStream.write(img->fileType[0]);
	bitStream.write(img->fileType[1]);
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
	else if(img->colorMode==ColorMode::DEDICATED)
		palette = img->dedicatedColorPalette;
	else if(img->colorMode==ColorMode::GREY_SCALE)
		palette = img->imposedGreyPalette;


	for(int i=0; i<palette.size(); ++i)
		bitStream.write(palette[i], sizeof(palette[i]));
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
	//TODO: jakby nie działało operator[] w bitsetach poniższych 3 metod to trzeba zmienic na metode test(i)
	writeWordsLength(word); 
	writeCodeWord(word);
	writePaletteIndex(word);
}

void XdFileWriter::writeWordsLength(const Word& word)
{
	std::bitset<6> length(word.codeWord.length());
	for(int i=0; i<length.size(); ++i)
		bitStream.writeBit(length[i]);
}

void XdFileWriter::writeCodeWord(const Word& word)
{
	std::bitset<63> codeWord(word.codeWord);

	size_t mostSignificantDigitPos=0;
	for(int i=63; i>=0; ++i)
	{
		if(codeWord.test(i)==true)
		{
			mostSignificantDigitPos=i;
			break;
		}
	}
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
	for(int i=0; i<img->compressedPixmap.size(); ++i)
	{
		std::string code=img->compressedPixmap[i];
		std::bitset<63> bitCode(code);
		size_t mostSignifDigPos=findMostSignificantDigit(bitCode);
		writeSinglePixel(bitCode, mostSignifDigPos);
	}
}

size_t XdFileWriter::findMostSignificantDigit(std::bitset<63>& bitCode)
{
	size_t mostSignificantDigitPos = 0;
	for(int i=63; i>=0; ++i)
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