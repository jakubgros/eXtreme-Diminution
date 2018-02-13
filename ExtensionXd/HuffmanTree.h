#pragma once
#include <string>
#include "Types.h"
#include "PixmapToStringConverter.h"

struct HTNode;


class HuffmanTree
{
public:
	explicit HuffmanTree(ImgWithParam* const img);
	~HuffmanTree();
	void run();
	Dictionary* getDictionary();
private:
	ImgWithParam* const img;
	std::string pixmap;
	PixmapToStringConverter ptsc;
	HTNode* root;
	Dictionary* dictionary;
private:
	static Palette* choosePalette(ImgWithParam* img);
	void makeList();
	void makeTree();

	void createDictionary(HTNode* p, std::string b);
public:
	void compress();
	void decompress();
};

