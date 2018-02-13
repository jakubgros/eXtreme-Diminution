#pragma once
#include "FileWriter.h"
#include <fstream>
#include "types.h"

class BmpFileWriter : public FileWriter
{
public:
	explicit BmpFileWriter(const ImgWithParam* img);
	~BmpFileWriter();
	bool write() override;
private:
	static const unsigned bfhSize = 14;
	static const unsigned bihSize = 40;
	static const unsigned fileSizeOffset = 2;
	static const unsigned pixmapOffset = 34;

	std::ofstream file_;
	BmpFileHeader bfh_;
	BmpImageHeader bih_;
	unsigned pixmapSize_;
	unsigned fileSize_;
private:
	void init();
	void initFile();
	void initFileHeader();
	void initImageHeader();
	void writeBmp();
	void writeFileHeader();
	void writeImageHeader();
	void writePixmap();
	int countPads() const;
	void writePixel(const int x, const int y);
	void writePads(const int nOfPads);
	void updatePixmapSize();
	unsigned countPixmapSize() const;
	void writePixmapSize();
	void updateFileSize();
	unsigned countFileSize() const;
	void writeFileSize();
private:
	BmpFileWriter(const BmpFileWriter&);
	BmpFileWriter& operator=(const BmpFileWriter&);
};