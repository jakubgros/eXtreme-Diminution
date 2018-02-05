#pragma once
#include "types.h"
#include "Decompressor.h"

class XdDecompressor : public Decompressor
{
public:
	explicit XdDecompressor(ImgWithParam* img);
	~XdDecompressor();
	virtual void decompress(const Dictionary* dictionary) override;
};

