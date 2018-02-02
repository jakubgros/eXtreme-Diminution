#pragma once
#include "Types.h"


class Decompressor
{
protected:
	ImgWithParam* img;
public:
	virtual ~Decompressor()=default;
	explicit Decompressor(ImgWithParam* img): img(img){};
	virtual void decompress(const Dictionary* dictionary)=0;
};

class XdDecompressor : public Decompressor
{
	struct DecodeTree
	{
		DecodeTree * next;
		DecodeTree * left;
		DecodeTree * right;
		int ColorNumber;
	};
	std::vector<std::string> CompressedPixmap;
public:
	virtual ~XdDecompressor() = default;
	explicit XdDecompressor(ImgWithParam* img);
	void decompress(const Dictionary* dictionary) override; //TODO: implement
	void MakeTree(DecodeTree * &root, const Dictionary* dictionary);
	void DecodeCode(DecodeTree* root, std::vector<std::string> CompressedPixmap, ImgWithParam* img);
	void DeleteTree(DecodeTree* root);
};

class BmpDecompressor : public Decompressor
{
public:
	virtual ~BmpDecompressor()=default;
	explicit BmpDecompressor(ImgWithParam* img): Decompressor(img){};
	void decompress(const Dictionary* dictionary=nullptr) override; //TODO: implement, dictionary parameter is not necessary, but added so that classes has the same interface 
};
