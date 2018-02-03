#pragma once
#include "Compressor.h"

class XdCompressor : public Compressor
{
	struct HuffmanNode
	{
		HuffmanNode* next;
		HuffmanNode* left;
		HuffmanNode* right;
		Rgb Color;
		int count;
	};
private:
	//	std::vector<std::string> CompressedPixmap;
	std::string s = "";
	float CompressRatio;
	Dictionary* dictionary;
public:
	explicit XdCompressor(ImgWithParam* img);
	virtual ~XdCompressor();
	virtual  Dictionary* compress() override;
private:
	void ReadHuffmanTree(HuffmanNode* &node, std::string code, Dictionary* dictrionary);
	void HuffmanList(HuffmanNode* &root, ImgWithParam* img);
	void HuffmanTree(HuffmanNode* &root);
	void CodeHuffman(HuffmanNode* root, std::string Code);
	bool CodePixmap(Rgb Color, HuffmanNode* root, std::string code);
	float CompressorRatio(ImgWithParam* img, std::vector<std::string> CompressedPixmap);
	int CheckColorNumber(ImgWithParam* img, Rgb Color);
	void DeleteTree(HuffmanNode* root);
};