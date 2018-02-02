#include "Decompressor.h"

explicit XdDecompressor::XdDecompressor(ImgWithParam* img) :
	Decompressor(img)
{

}


void XdDecompressor::decompress(const Dictionary* dictionary)
{
	DecodeTree* root;
	MakeTree(root, dictionary);
	DecodeCode(root, CompressedPixmap, img);
	DeleteTree(root);

}


void XdDecompressor::MakeTree(DecodeTree * &root,const Dictionary* dictionary)
{
	int colorNumber;
	std::string Word;
	DecodeTree* Temp;

	root = new DecodeTree;
	root->left = root->right = NULL;



	for (int i = 0; i < dictionary->size(); ++i)
		for (int j = 0; j < dictionary[i].size(); ++j)
		{
			Word = dictionary[i][j]->codeWord;
			colorNumber = dictionary[i][j]->number;

			Temp = root;

			for (int k = 0; k < (int)Word.length(); ++k)
				if (Word[k] == '0')
				{
					if (!Temp->left)
					{
						Temp->left = new DecodeTree;
						Temp->left->left = NULL;
						Temp->left->right = NULL;
					}
					Temp = Temp->left;
				}
				else
				{
					if (!Temp->right)
					{
						Temp->right = new DecodeTree;
						Temp->right->left = NULL;
						Temp->right->right = NULL;
					}
					Temp = Temp->right;
				}

			Temp->ColorNumber = colorNumber;

		}
}


void XdDecompressor::DecodeCode(DecodeTree* root, std::vector<std::string> CompressedPixmap, ImgWithParam* img)
{
	DecodeTree* currentNode;
	std::string Code;
	int currentHeight = 0;
	int currentWidth = 0;

	for (int i = 0; i < CompressedPixmap.size(); ++i)
	{
		Code += CompressedPixmap[i];
	}

	currentNode = root;

	for (int i = 0; i < (int)Code.length(); i++)
	{
		if (Code[i] == '0')
			currentNode = currentNode->left;
		else
			currentNode = currentNode->right;
		if (!currentNode->left)
		{
			if (currentWidth >= img->width)
				++currentHeight;
			if(img->colorMode==GREY_SCALE)
				img->pixmap[currentHeight][currentWidth] = img->imposedGreyPalette[currentNode->ColorNumber]; 
			else if(img->colorMode==IMPOSED)
				img->pixmap[currentHeight][currentWidth] = img->imposedGreyPalette[currentNode->ColorNumber];
			else if(img->colorMode==DEDICATED)
				img->pixmap[currentHeight][currentWidth] = img->dedicatedColorPalette[currentNode->ColorNumber];

			++currentWidth;
			currentNode = root;
		}
	}
}

void XdDecompressor::DeleteTree(DecodeTree* root)
{
	if (root != NULL)
	{
		DeleteTree(root->left);
		DeleteTree(root->right);
		delete root;
	}
}


void BmpDecompressor::decompress(const Dictionary* dictionary)
{
	// not needed
}
