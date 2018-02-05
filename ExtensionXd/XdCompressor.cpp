#include "XdCompressor.h"


XdCompressor::XdCompressor(ImgWithParam* img) :
	Compressor(img),
	ht(img)
{

}

Dictionary* XdCompressor::compress()
{
	ht.run();
	ht.compress();
	return ht.getDictionary();
}

XdCompressor::~XdCompressor()
{
}
