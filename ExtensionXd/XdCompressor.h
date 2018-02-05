#pragma once
#include  "types.h"
#include "HuffmanTree.h"
#include  "Compressor.h"

class XdCompressor : public Compressor
{
public:
	explicit XdCompressor(ImgWithParam* img);
	~XdCompressor();
	Dictionary* compress();
private:
	Dictionary* dictionary;
	HuffmanTree ht;

};

