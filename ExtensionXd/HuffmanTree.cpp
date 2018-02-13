#include "HuffmanTree.h"
#include  <iostream> // usunac jesli nie ma wyjatkow

struct HTNode
{
	HTNode* next;
	HTNode* left;
	HTNode* right;
	char ch[2];
	int count;
};


HuffmanTree::HuffmanTree(ImgWithParam* const img) :
	img(img),
	pixmap(""),
	ptsc(&img->pixmap, choosePalette(img)),
	root(nullptr),
	dictionary(new Dictionary)
{

}



HuffmanTree::~HuffmanTree()
{
}

void HuffmanTree::run()
{
	pixmap = ptsc.convert();
	makeList();
	makeTree();
	createDictionary(root, "0");
//	std::cout << "compressed" << std::endl;
//	compress();
//	decompress();
}

Palette* HuffmanTree::choosePalette(ImgWithParam* img)
{
	switch(img->colorMode)
	{
	case DEDICATED:
		return &img->dedicatedColorPalette;
	case IMPOSED:
		return &img->imposedColorPalette;
	case GREY_SCALE:
		return &img->imposedGreyPalette;
	default:
		return nullptr;
	}
}

void HuffmanTree::makeList()
{
	unsigned long i, x;
	char cx[2];
	HTNode* p;
	bool t;
	root = nullptr;
	for(i=0; i< pixmap.length(); i+=2)
	{
		p = root;
		while (p && (p->ch[0] != pixmap[i] || p->ch[1] != pixmap[i+1]))
			p = p->next;
		if(!p)
		{
			p = new HTNode;
			p->next = root;
			p->left = nullptr;
			p->right = nullptr;
			p->ch[0] = pixmap[i];
			p->ch[1] = pixmap[i + 1];
			p->count = 0;
			root = p;
		}
		p->count++;
	}

	do
	{
		t = true;
		p = root;
		while(p->next)
		{
			if(p->count > p->next->count)
			{
				cx[0] = p->ch[0];
				cx[1] = p->ch[1];
				p->ch[0] = p->next->ch[0];
				p->ch[1] = p->next->ch[1];
				p->next->ch[0] = cx[0];
				p->next->ch[1] = cx[1];
				x = p->count;
				p->count = p->next->count;
				p->next->count = x;
				t = false;
			}
			p = p->next;
		}
	} while (!t);
}

void HuffmanTree::makeTree()
{
	HTNode *p, *r, *v1, *v2;

	while (true)
	{
		v1 = root;
		v2 = v1->next;

		if (!v2)
			break;

		root = v2->next;

		p = new HTNode;
		p->left = v1;
		p->right = v2;
		p->count = v1->count + v2->count;

		if (!root || (p->count <= root->count))
		{
			p->next = root;
			root = p;
			continue;
		}

		r = root;

		while (r->next && (p->count > r->next->count))
			r = r->next;

		p->next = r->next;
		r->next = p;
	}
}

void HuffmanTree::createDictionary(HTNode* p, std::string b)
{
	if (!p->left)
	{
		//dodawanie do slownika
		int g = (p->ch[0]-'0')*10 + (p->ch[1]-'0');
		const Word word = { g, b };
		dictionary->push_back(word);
//		std::cout << g << " " << b << std::endl;
	}
	else
	{
		createDictionary(p->left, b + "0");
		createDictionary(p->right, b + "1");
	}
}

Dictionary* HuffmanTree::getDictionary()
{
	return dictionary;
}

void HuffmanTree::compress()
{
	char c[2];
	for(int i=0; i< pixmap.length(); i+=2)
	{
		c[0] = pixmap[i];
		c[1] = pixmap[i + 1];
		int g = (c[0] - '0') * 10 + (c[1] - '0');
		bool isFound = false;
		for(int color = 0; color < dictionary->size(); ++color)
		{
			if (g == (*dictionary)[color].number)
			{
				img->compressedPixmap.push_back((*dictionary)[color].codeWord);
				isFound = true;
				break;
				//std::cout << (*dictionary)[color].codeWord << " ";
			}
		}
		if (!isFound)
			throw std::logic_error("HuffmanTree::compress() - no such color in palette");
		isFound = false;
	}
}

void HuffmanTree::decompress()
{
//	std::cout << std::endl;
//	std::cout << "Decompressed" << std::endl;
	for(int i=0; i < img->compressedPixmap.size(); ++i)
	{
		for (int j = 0; j < dictionary->size(); ++j)
		{
			if(img->compressedPixmap[i] == (*dictionary)[j].codeWord)
			{
			//	std::cout << static_cast<int>((*dictionary)[j].number) << " ";
				break;
			}
		}
	}
}

