#pragma once
#include "Decompressor.h"

class XdDecompressor : public Decompressor
{
	struct DecodeTree
	{
		DecodeTree * next;
		DecodeTree * left;
		DecodeTree * right;
		int ColorNumber;
	};
	//	std::vector<std::string> CompressedPixmap;
public:
	virtual ~XdDecompressor() = default;
	explicit XdDecompressor(ImgWithParam* img);
	void initPixmap();
	void decompress(const Dictionary* dictionary) override; //TODO: implement
	void MakeTree(DecodeTree * &root, const Dictionary* dictionary);
	void DecodeCode(DecodeTree* root, std::vector<std::string> CompressedPixmap, ImgWithParam* img);
	void DeleteTree(DecodeTree* root);
};