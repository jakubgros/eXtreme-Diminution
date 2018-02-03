#pragma once
#include "FileWriter.h"


class BmpFileWriter : public FileWriter
{
public:
	explicit BmpFileWriter(const ImgWithParam* img);
	virtual ~BmpFileWriter() = default;
	bool write() override;
private:
	bitmap_file_header bfh;
	bitmap_image_header bih;
	const int file_size;
	const int dpi;
	const int ppm;
	FILE* image;
private:
	void init();
	void initBfh();
	void initBih();
	void initBmpFile();
	void writePixmap();
};

