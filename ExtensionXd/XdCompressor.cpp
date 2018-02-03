#include "XdCompressor.h"

XdCompressor::XdCompressor(ImgWithParam* img) :
	Compressor(img),
	dictionary(new Dictionary)
{

}

XdCompressor::~XdCompressor()
{
	delete dictionary;
}

Dictionary* XdCompressor::compress()
{

	HuffmanNode* root;
	HuffmanList(root, img);
	HuffmanTree(root);
	ReadHuffmanTree(root, s, dictionary);
	CodeHuffman(root, s);
	CompressRatio = CompressorRatio(img, img->compressedPixmap);
	DeleteTree(root);
	return dictionary;
}

void XdCompressor::HuffmanList(HuffmanNode* &root, ImgWithParam* img)
{
	HuffmanNode *PresentNode;
	int help;
	bool IsSorted;
	Rgb Temp;

	root = NULL;

	for (int y = 0; y < img->height; ++y)
	{
		for (int x = 0; x < img->width; ++x)
		{
			PresentNode = root;
			while (PresentNode != NULL && PresentNode->Color != img->pixmap[x][y]) PresentNode = PresentNode->next;
			if (PresentNode == NULL)
			{
				PresentNode = new HuffmanNode;
				PresentNode->left = NULL;
				PresentNode->right = NULL;
				PresentNode->next = root;
				PresentNode->Color = img->pixmap[x][y];
				PresentNode->count = 0;
				root = PresentNode;
			}
			PresentNode->count++;
		}
	}

	do
	{
		IsSorted = true;
		PresentNode = root;
		while (PresentNode->next != NULL)
		{
			if (PresentNode->count > PresentNode->next->count)
			{
				Temp = PresentNode->Color;
				PresentNode->Color = PresentNode->next->Color;
				PresentNode->next->Color = Temp;
				help = PresentNode->count;
				PresentNode->count = PresentNode->next->count;
				PresentNode->next->count = help;
				IsSorted = false;
			}
			PresentNode = PresentNode->next;
		}
	} while (!IsSorted);
}

void XdCompressor::HuffmanTree(HuffmanNode* &root)
{
	HuffmanNode *Temp, *r, *FirstNode, *SecondNode;

	while (true)
	{
		FirstNode = root;
		SecondNode = FirstNode->next;

		if (!SecondNode) break;

		root = SecondNode->next;

		Temp = new HuffmanNode;
		Temp->left = FirstNode;
		Temp->right = SecondNode;
		Temp->count = FirstNode->count + SecondNode->count;


		if (!root || (Temp->count <= root->count))
		{
			Temp->next = root;
			root = Temp;
			continue;
		}

		r = root;
		while (r->next && (Temp->count > r->next->count)) r = r->next;

		Temp->next = r->next;
		r->next = Temp;
	}
}

void XdCompressor::ReadHuffmanTree(HuffmanNode* &root, std::string Code, Dictionary* dictionary)
{
	if (root->left == NULL)
	{
		int i = CheckColorNumber(img, root->Color);
		Word word;
		word.codeWord = Code;
		word.number = i;
		dictionary->push_back(word);
	}
	else
	{
		ReadHuffmanTree(root->left, Code + "0", dictionary);
		ReadHuffmanTree(root->right, Code + "1", dictionary);
	}
}

int XdCompressor::CheckColorNumber(ImgWithParam* img, Rgb Color)
{
	if (img->colorMode == ColorMode::DEDICATED)
		for (int i = 0; i < img->imposedColorPalette.size(); ++i)
			if (Color == img->imposedColorPalette[i])
				return i;

	if (img->colorMode == ColorMode::GREY_SCALE)
		for (int i = 0; i < img->imposedGreyPalette.size(); ++i)
			if (Color == img->imposedGreyPalette[i])
				return i;

	if (img->colorMode == ColorMode::IMPOSED)
		for (int i = 0; i < img->dedicatedColorPalette.size(); ++i)
			if (Color == img->dedicatedColorPalette[i])
				return i;
}

void XdCompressor::CodeHuffman(HuffmanNode* root, std::string s)
{
	for (int y = 0; y < img->height; ++y)
		for (int x = 0; x < img->width; ++x)
			CodePixmap(img->pixmap[x][y], root, "");
}

bool XdCompressor::CodePixmap(Rgb Color, HuffmanNode* root, std::string code)
{
	if (root->left != NULL)
	{
		if (Color != root->Color)
			return false;
		else
		{
			img->compressedPixmap.push_back(code);
			return true;
		}
	}
	else return CodePixmap(Color, root->left, code + "0") || CodePixmap(Color, root->right, code + "1");
}

float XdCompressor::CompressorRatio(ImgWithParam* img, std::vector<std::string> CompressedPixmap)
{
	int PixmapCapacity = 0;
	int CompressedCapacity = img->compressedPixmap.capacity();
	for (int i = 0; i < img->height; ++i) PixmapCapacity += img->pixmap.capacity();

	return (float)CompressedCapacity / PixmapCapacity * 100;
}

void XdCompressor::DeleteTree(HuffmanNode* root)
{
	if (root != NULL)
	{
		DeleteTree(root->left);
		DeleteTree(root->right);
		delete root;
	}
}