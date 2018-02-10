#include "BmpFileWriter.h"
#include <vector>
#include <fstream>

BmpFileWriter::BmpFileWriter(const ImgWithParam* img) :
	FileWriter(img),
	data(),
	outputDataSeries(nullptr),
	file(),
	bfh(),
	bih()
{

}

bool BmpFileWriter::write()
{
	init();
	createOutputData();
	createOutputDataSeries();
	writeBmp();
	delete[] outputDataSeries;
	return true;
}

void BmpFileWriter::init()
{
	initFileHeader();
	initImageHeader();
}

void BmpFileWriter::initFileHeader()
{
	bfh.bitmapType[0] = 'B';
	bfh.bitmapType[1] = 'M';
	bfh.fileSize = 0;
	bfh.reserved = 0;
	bfh.offsetBits = 0;
}

void BmpFileWriter::initImageHeader()
{
	bih.bitmapInfoHeader = 40;
	bih.width = img->width;
	bih.height = img->height;
	bih.planes = 1;
	bih.bitCount = 24;
	bih.compression = 0;
	bih.imageSize = 0;
	bih.horizontalRes = 2835;
	bih.verticalRes = 2835;
	bih.clrUsed = 0;
	bih.clrImportant = 0;
}

void BmpFileWriter::createOutputData()
{
	putBytes(bfhSize, bfh);
	putBytes(bihSize, bih);
	putPixmap();
	updateFileSize();
	updatePixmapSize();
	updateFileSize();
	updatePixmapSize();
}

template <typename T>
void BmpFileWriter::putBytes(const unsigned nOfBytes, const T& source)
{
	uint8_t* dividedSource = (uint8_t*)&source;
	for (size_t i = 0; i < nOfBytes; ++i)
		data.push_back(dividedSource[i]);
}

void BmpFileWriter::putBGR(const int x, const int y)
{
	data.push_back(static_cast<uint8_t>(img->pixmap[x][y].b));
	data.push_back(static_cast<uint8_t>(img->pixmap[x][y].g));
	data.push_back(static_cast<uint8_t>(img->pixmap[x][y].r));
}

void BmpFileWriter::putPadding()
{
	while ((data.size() - bfhSize - bihSize) % 4)
		data.push_back(0);
}

void BmpFileWriter::putPixmap()
{
	for (size_t y = 0; y < img->height; ++y)
	{
		for (size_t x = 0; x < img->width; ++x)
			putBGR(x, y);
		putPadding();
	}
}

void BmpFileWriter::updateFileSize()
{
	uint32_t fileSize = data.size();
	memcpy(&data[fileSizeOffset], &fileSize, 4);
}

void BmpFileWriter::updatePixmapSize()
{
	uint32_t pixmapSize = data.size() - bfhSize - bihSize;
	memcpy(&data[pixmapOffset], &pixmapSize, 4);
}

void BmpFileWriter::createOutputDataSeries()
{
	outputDataSeries = new uint8_t[data.size()];
	memcpy(outputDataSeries, &data[0], data.size());
}

void BmpFileWriter::writeBmp()
{
	file.open(img->outputFilepath);
	file.write((const char*)outputDataSeries, data.size());
	file.close();
}