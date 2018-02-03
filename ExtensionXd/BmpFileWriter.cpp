#include "BmpFileWriter.h"
#include "Types.h"
#include <fstream>


BmpFileWriter::BmpFileWriter(const ImgWithParam* img) :
	FileWriter(img),
	file_size(54 + 4 * img->width*img->height),
	dpi(96),
	ppm(dpi * 39.375)
{
	init();
}

void BmpFileWriter::init()
{
	initBfh();
	initBih();
	initBmpFile();
}

void BmpFileWriter::initBfh()
{
	memcpy(&bfh.bitmap_type, "BM", 2);
	bfh.fileSize = file_size;
	bfh.reserved1 = 0;
	bfh.reserved2 = 0;
	bfh.offsetBits = 0;
}

void BmpFileWriter::initBih()
{
	bih.sizeHeader = sizeof(bih);
	bih.width = img->width;
	bih.height = img->height;
	bih.planes = 1;
	bih.bitCount = 24;
	bih.compression = 0;
	bih.imageSize = file_size;
	bih.ppmX = ppm;
	bih.ppmY = ppm;
	bih.clrUsed = 0;
	bih.clrImportant = 0;
}

void BmpFileWriter::initBmpFile()
{
	std::string filePath = img->outputFilepath;
	fopen_s(&image, filePath.c_str(), "wb");
	if (image == NULL)
	{
		std::string* msg = new std::string("Cannot create BMP file");
		throw std::exception(msg->c_str());
	}
}

void BmpFileWriter::writePixmap()
{
	for (int y =  0; y < img->height; ++y)
		for (int x = 0; x < img->width; ++x)
		{
			Rgb currentColor = img->pixmap[x][y];
			unsigned char color[3] = { currentColor.b,currentColor.g,currentColor.r };
			fwrite(color, 1, sizeof(color), image);
		}
}

bool BmpFileWriter::write()
{
	fwrite(&bfh, 1, 14, image);
	fwrite(&bih, 1, sizeof(bih), image);
	writePixmap();
	if (fclose(image) == EOF)
	{
		std::string* msg = new std::string("Cannot close BMP file");
		throw std::exception(msg->c_str());
	}
	return true;
}