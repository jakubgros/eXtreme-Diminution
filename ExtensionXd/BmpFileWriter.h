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
	const ImgWithParam* img;
	std::vector<uint8_t> data;
	size_t fileSizeOffset;
	size_t pixmapOffset;
	BmpFileHeader bfh;
private:
	void addFileHeader();
	void writePixmap();
	size_t encodeRgb(uint8_t** output);
};