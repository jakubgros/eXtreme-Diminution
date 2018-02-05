#include "XdDecompressor.h"
#include <iostream> //delete if no exception thrown


XdDecompressor::XdDecompressor(ImgWithParam* img) :
	Decompressor(img)
{
}


XdDecompressor::~XdDecompressor()
{
}

void XdDecompressor::decompress(const Dictionary* dictionary)
{
	//std::cout << std::endl;
	//std::cout << "Decompressed" << std::endl;
	for (int i = 0; i < img->compressedPixmap.size(); ++i)
	{
		for (int j = 0; j < dictionary->size(); ++j)
		{
			if (img->compressedPixmap[i] == (*dictionary)[j].codeWord)
			{
				switch (img->colorMode)
				{
				case IMPOSED:
					img->pixmap[i%img->width][i / (img->width)] = img->imposedColorPalette[(*dictionary)[j].number];
					break;
				case DEDICATED:
					img->pixmap[i%img->width][i / (img->width)] = img->dedicatedColorPalette[(*dictionary)[j].number];
					break;
				case GREY_SCALE:
					img->pixmap[i%img->width][i / (img->width)] = img->imposedGreyPalette[(*dictionary)[j].number];
					break;
				}
				//std::cout << static_cast<int>((*dictionary)[j].number) << " ";
				break;
			}
		}
	}
}


