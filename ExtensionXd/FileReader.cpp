#include "FileReader.h"

XdFileReader::~XdFileReader()
{
	delete dictionary;
}

const Dictionary * XdFileReader::getDictionary()
{
	return dictionary;
}

