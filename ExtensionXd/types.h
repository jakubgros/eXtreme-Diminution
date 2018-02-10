#pragma once
#include <string>
#include <vector>
#include <array>

//TODO: change name of file

enum ColorMode
{
	GREY_SCALE,
	DEDICATED,
	IMPOSED
};

struct Rgb
{
	int r;
	int g;
	int b;
};

struct Lab
{
	double l;
	double a;
	double b;
};

struct Word //TODO: check if types are ok
{
	int number;
	std::string codeWord;
};

typedef std::vector<std::vector<Rgb>> Pixmap;
typedef std::array<Rgb, 64> Palette;
typedef std::vector<Word> Dictionary;

struct ImgWithParam
{
	int width;
	int height;
	Pixmap pixmap;
	ColorMode colorMode;
	std::string fileType;
	Palette dedicatedColorPalette;
	Palette imposedColorPalette;
	Palette imposedGreyPalette;
	long long fileSize;
	std::string inputFilepath;
	std::string outputFilepath;
	std::vector<std::string> compressedPixmap;
};

struct BmpFileHeader
{
	unsigned char bitmapType[2];
	unsigned int fileSize;
	int reserved;
	unsigned int offsetBits;
};

struct BmpImageHeader
{
	unsigned bitmapInfoHeader;
	unsigned width;
	unsigned height;
	short planes;
	short bitCount;
	unsigned compression;
	unsigned imageSize;
	unsigned horizontalRes;
	unsigned verticalRes;
	unsigned clrUsed;
	unsigned clrImportant;
};

Rgb operator+ (const Rgb &left, const Rgb& right);

Rgb operator-(const Rgb &left, const Rgb& right);

Rgb operator*(const double &left, const Rgb& right);

Rgb operator*(const Rgb& left, const double& right);

bool operator !=(const Rgb& left,const Rgb& right);

bool operator ==(const Rgb& left,const Rgb& right);