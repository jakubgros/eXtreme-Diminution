#include "BmpFileWriter.h"
#include <vector>

BmpFileWriter::BmpFileWriter(const ImgWithParam* img) :
	FileWriter(img),
	file_(),
	bfh_(),
	bih_(),
	pixmapSize_(0),
	fileSize_(0)
{

}

BmpFileWriter::~BmpFileWriter()
{
	if(file_.is_open())
		file_.close();
}

bool BmpFileWriter::write()
{
	try
	{
		init();
		writeBmp();
	}
	catch (std::exception& e)
	{
		return false;
	}
	return true;
}


void BmpFileWriter::init()
{
	initFile();
	initFileHeader();
	initImageHeader();
}

void BmpFileWriter::initFile()
{
	file_.open(img->outputFilepath, std::ios::out | std::ios::binary);
}

void BmpFileWriter::initFileHeader()
{
	bfh_.bitmapType[0] = 'B';
	bfh_.bitmapType[1] = 'M';
	bfh_.fileSize = 0;
	bfh_.reserved = 0;
	bfh_.offsetBits = 54;
}

void BmpFileWriter::initImageHeader()
{
	bih_.bitmapInfoHeader = 40;
	bih_.width = img->width;
	bih_.height = img->height;
	bih_.planes = 1;
	bih_.bitCount = 24;
	bih_.compression = 0;
	bih_.imageSize = 0;
	bih_.horizontalRes = 2835;
	bih_.verticalRes = 2835;
	bih_.clrUsed = 0;
	bih_.clrImportant = 0;
}

void BmpFileWriter::writeBmp()
{
	writeFileHeader();
	writeImageHeader();
	writePixmap();
	updatePixmapSize();
	updateFileSize();
}

void BmpFileWriter::writeFileHeader()
{
	file_.write(reinterpret_cast<const char*>(&bfh_.bitmapType), 2);
	file_.write(reinterpret_cast<const char*>(&bfh_.fileSize), 4);
	file_.write(reinterpret_cast<const char*>(&bfh_.reserved), 4);
	file_.write(reinterpret_cast<const char*>(&bfh_.offsetBits), 4);
}

void BmpFileWriter::writeImageHeader()
{
	file_.write(reinterpret_cast<const char*>(&bih_.bitmapInfoHeader), 4);
	file_.write(reinterpret_cast<const char*>(&bih_.width), 4);
	file_.write(reinterpret_cast<const char*>(&bih_.height), 4);
	file_.write(reinterpret_cast<const char*>(&bih_.planes), 2);
	file_.write(reinterpret_cast<const char*>(&bih_.bitCount), 2);
	file_.write(reinterpret_cast<const char*>(&bih_.compression), 4);
	file_.write(reinterpret_cast<const char*>(&bih_.imageSize), 4);
	file_.write(reinterpret_cast<const char*>(&bih_.horizontalRes), 4);
	file_.write(reinterpret_cast<const char*>(&bih_.verticalRes), 4);
	file_.write(reinterpret_cast<const char*>(&bih_.clrUsed), 4);
	file_.write(reinterpret_cast<const char*>(&bih_.clrImportant), 4);
}

void BmpFileWriter::writePixmap()
{
	const int nOfPads = countPads();
	for (int y = 0; y < img->height; ++y)
	{
		for (int x = 0; x < img->width; ++x)
			writePixel(x, y);
		writePads(nOfPads);
	}
}

int BmpFileWriter::countPads() const
{
	const int nOfPads = 4 - (img->width * 3) % 4;
	return ((nOfPads == 4) ? 0 : nOfPads);
}

void BmpFileWriter::writePixel(const int x, const int y)
{
	file_.write(reinterpret_cast<const char*>(&img->pixmap[x][y].b), 1);
	file_.write(reinterpret_cast<const char*>(&img->pixmap[x][y].g), 1);
	file_.write(reinterpret_cast<const char*>(&img->pixmap[x][y].r), 1);
}

void BmpFileWriter::writePads(const int nOfPads)
{
	const int pad = 0;
	for (int i = 0; i < nOfPads; ++i)
		file_.write(reinterpret_cast<const char*>(&pad), 1);
}

void BmpFileWriter::updatePixmapSize()
{
	pixmapSize_ = countPixmapSize();
	writePixmapSize();
}

unsigned BmpFileWriter::countPixmapSize() const
{
	return img->height * (img->width * 3 + 4-(img->width * 3) % 4);
}

void BmpFileWriter::writePixmapSize()
{
	const std::streampos init = file_.tellp();
	file_.seekp(pixmapOffset);
	file_.write(reinterpret_cast<const char*>(&pixmapSize_), 4);
	file_.seekp(init);
}

void BmpFileWriter::updateFileSize()
{
	fileSize_ = countFileSize();
	writeFileSize();
}

unsigned BmpFileWriter::countFileSize() const
{
	return pixmapSize_ + bihSize + bfhSize;
}

void BmpFileWriter::writeFileSize()
{
	const std::streampos init = file_.tellp();
	file_.seekp(fileSizeOffset);
	file_.write(reinterpret_cast<const char*>(&fileSize_), 4);
	file_.seekp(init);
}
