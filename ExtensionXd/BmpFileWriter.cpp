#include "BmpFileWriter.h"
#include <vector>
#include <fstream>

BmpFileWriter::BmpFileWriter(const ImgWithParam* img) :
	FileWriter(img),
	data(),
	pixmapOffset(0),
	bfh()
{

}

std::vector<uint8_t> get4Bytes(const uint32_t source)
{
	std::vector<uint8_t> result;
	uint8_t* dividedSource = (uint8_t*)&source;
	result.push_back(dividedSource[0]);
	result.push_back(dividedSource[1]);
	result.push_back(dividedSource[2]);
	result.push_back(dividedSource[3]);
	return result;
}
void put4Bytes(const uint32_t source, std::vector<uint8_t>& destination)
{
	std::vector<uint8_t> separatedData = get4Bytes(source);
	std::copy(separatedData.begin(), separatedData.end(), back_inserter(destination));
}

std::vector<uint8_t> get2Bytes(const uint16_t source)
{
	std::vector<uint8_t> result;
	uint8_t* dividedSource = (uint8_t*)&source;
	result.push_back(dividedSource[0]);
	result.push_back(dividedSource[1]);
	return result;
}
void put2Bytes(const uint16_t & data, std::vector<uint8_t> & dest)
{
	std::vector<uint8_t> separated_data = get2Bytes(data);
	copy(separated_data.begin(), separated_data.end(), back_inserter(dest));
}

void BmpFileWriter::addFileHeader()
{
	uint8_t* dividedSoource = (uint8_t*)&bfh;
	for (size_t i = 0; i < sizeof(bfh); ++i)
	{
		data.push_back(dividedSoource[i]);
	}
}

size_t BmpFileWriter::encodeRgb(uint8_t** output)
{
	bfh.bitmapType[0] = 'B';
	bfh.bitmapType[1] = 'M';
	bfh.fileSize = 0;
	bfh.offsetBits = 0;

	data.push_back(bfh.bitmapType[0]);
	data.push_back(bfh.bitmapType[1]);
	fileSizeOffset = data.size();
	put4Bytes(bfh.fileSize, data);
	put4Bytes(bfh.reserved, data);
	pixmapOffset = data.size();
	put4Bytes(0, data); //pixel info offset, fill later

	fileSizeOffset = 2;
	pixmapOffset = 10;

	BmpImageHeader bih;
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

	put4Bytes(40, data); //Size of BITMAPINFOHEADER
	put4Bytes(img->width, data);
	put4Bytes(img->height, data);
	put2Bytes(1, data); //Number of color planes
	put2Bytes(24, data); //Bits per pixel
	put4Bytes(0, data); //No compression
	pixmapOffset = data.size();
	put4Bytes(0, data); //size of raw data in pixel array, fill later
	put4Bytes(2835, data); //Horizontal Resolution
	put4Bytes(2835, data); //Vertical Resolution
	put4Bytes(0, data); //Number of colors
	put4Bytes(0, data); //Important colors
	{
		uint32_t data_size = data.size();
		memcpy(&data[pixmapOffset], &data_size, 4);
	}
	uint32_t size_of_header = data.size();
	for (uint_fast32_t y = 0; y < img->height; ++y)
	{
		for (uint_fast32_t x = 0; x < img->width; ++x)
		{
			uint8_t r = img->pixmap[x][y].r;
			uint8_t g = img->pixmap[x][y].g;
			uint8_t b = img->pixmap[x][y].b;
			data.push_back(b);
			data.push_back(g);
			data.push_back(r);
		}
		while ((data.size() - size_of_header) % 4)
		{
			data.push_back(0);
		}
	}
	{
		uint32_t file_size = data.size();
		memcpy(&data[fileSizeOffset], &file_size, 4);
	}
	{
		uint32_t pixel_data_size = data.size() - size_of_header;
		memcpy(&data[pixmapOffset], &pixel_data_size, 4);
	}
	*output = new uint8_t[data.size()];
	memcpy(*output, &data[0], data.size());
	return data.size();
}

bool BmpFileWriter::write()
{
	uint8_t* processedPixmap;
	size_t output_size = encodeRgb(&processedPixmap);
	std::ofstream file_output;
	file_output.open(img->outputFilepath);
	file_output.write((const char*)processedPixmap, output_size);
	file_output.close();
	delete[] processedPixmap;
	return true;
}