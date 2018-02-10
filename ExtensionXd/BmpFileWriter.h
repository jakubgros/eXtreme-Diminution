#pragma once
#include "Types.h"
#include <fstream>
#include "FileWriter.h"

class BmpFileWriter : public FileWriter
{
public:
	explicit BmpFileWriter(const ImgWithParam* img);
	bool write();
private:
	static const unsigned bfhSize = 14;
	static const unsigned bihSize = 40;
	static const unsigned fileSizeOffset = 2;
	static const unsigned pixmapOffset = 34;

	std::vector<uint8_t> data;
	uint8_t* outputDataSeries;
	std::ofstream file;
	BmpFileHeader bfh;
	BmpImageHeader bih;
private:
	void init();
	void initFileHeader();
	void initImageHeader();
	void createOutputData();
	void createOutputDataSeries();
	void writeBmp();
	template<typename T>
	void putBytes(const unsigned nOfBytes, const T& source);
	void putPixmap();
	void putBGR(const int x, const int y);
	void putPadding();
	void updateFileSize();
	void updatePixmapSize();
};