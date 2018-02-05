#pragma once
#include <string>
 //Dictionary


struct ImgWithParam;
class Compressor;
class FileWriter;
class Validator;
class FileReader;
class Decompressor;
class Converter;
enum ColorMode;

enum FamilyType
{
	BMP,
	XD
};
 
class ObjectCreator
{
	FamilyType loadedFileType;
public:
	explicit ObjectCreator(FamilyType type): loadedFileType(type){};

	FileWriter* createFileWriter(const ImgWithParam* img);
	Compressor* createCompressor(ImgWithParam* imgWithParam);

	Validator* createValidator(ImgWithParam* imgWithParam);
	FileReader* createFileReader(ImgWithParam* imgWithParam);
	Decompressor* createDecompressor(ImgWithParam* imgWithParam);

	Converter* createConverter(ImgWithParam* imgWithParam, ColorMode colorMode);

};