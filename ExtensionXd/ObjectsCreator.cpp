#include "ObjectsCreator.h"
#include "FileWriter.h"
#include "Compressor.h"
#include <exception>
#include "Validator.h"
#include "FileReader.h"
#include "Decompressor.h"
#include "Converter.h"

FileWriter* ObjectCreator::createFileWriter(const ImgWithParam* img)
{
	if(type==BMP)
		return new BmpFileWriter(img);
	if(type==XD)
		return new XdFileWriter(img);
}

Compressor* ObjectCreator::createCompressor(ImgWithParam* imgWithParam)
{
	if(type==BMP)
		return new BmpCompressor(imgWithParam);
	if(type==XD)
		return new XdCompressor(imgWithParam);
}

Validator* ObjectCreator::createValidator(ImgWithParam* imgWithParam)
{
	if(type==BMP)
		return new BmpValidator(imgWithParam);
	if(type==XD)
		return new XdValidator(imgWithParam);
}

FileReader* ObjectCreator::createFileReader(ImgWithParam* imgWithParam)
{
	if(type==BMP)
		return new BmpFileReader(imgWithParam);
	if(type==XD)
		return new XdFileReader(imgWithParam);
}

Decompressor* ObjectCreator::createDecompressor(ImgWithParam* imgWithParam)
{
	if(type==BMP)
		return new BmpDecompressor(imgWithParam);
	if(type==XD)
		return new XdDecompressor(imgWithParam);
}

Converter* ObjectCreator::createConverter(ImgWithParam* imgWithParam, ColorMode colorMode)
{
	if(type==BMP) //has loaded *.bmp file so it will be converted to xd extension
	{
		if(colorMode==DEDICATED)
			return new ToDedicatedPixmapConverter(imgWithParam);
		if(colorMode==GREY_SCALE)
			return new ToGreyScalePixmapConverter(imgWithParam);
		if(colorMode==IMPOSED)
			return new ToImposedPixmapConverter(imgWithParam);
	}
	if(type==XD)
		return new XdToBmpConverter(imgWithParam, colorMode);
}
