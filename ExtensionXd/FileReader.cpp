#include "FileReader.h"

XdFileReader::~XdFileReader()
{
	delete dictionary;
}

void XdFileReader::read()
{
	//TODO: wczytuje z pliku podanego jako input w img i zapisuje wszystko do img, a slownik do dictionary

}

const Dictionary * XdFileReader::getDictionary()
{
	return dictionary;
}

