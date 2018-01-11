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
};
