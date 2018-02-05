#include "ObjectsCreator.h"
#include "FileWriter.h"
#include "BmpFileWriter.h"
#include "Compressor.h"
#include "XdCompressor.h"
#include <exception>
#include "Validator.h"
#include "FileReader.h"
#include "BmpFileReader.h"
#include "Decompressor.h"
#include "XdDecompressor.h"
#include "Converter.h"
#include "ToDedicatedPixmapConverter.h"
#include "ToGreyScalePixmapConverter.h"
#include "ToImposedPixmapConverter.h"
#include "XdToBmpConverter.h"
#include "XdFileWriter.h"
#include "XdFileReader.h"

FileWriter* ObjectCreator::createFileWriter(const ImgWithParam* img)
{
	if(loadedFileType==BMP)
		return new XdFileWriter(img);
	if(loadedFileType==XD)
		return new BmpFileWriter(img);
}

Compressor* ObjectCreator::createCompressor(ImgWithParam* imgWithParam)
{
	if(loadedFileType==BMP)
		return new XdCompressor(imgWithParam);
	if(loadedFileType==XD)
		return new BmpCompressor(imgWithParam);
}

Validator* ObjectCreator::createValidator(ImgWithParam* imgWithParam)
{
	if(loadedFileType==BMP)
		return new BmpValidator(imgWithParam);
	if(loadedFileType==XD)
		return new XdValidator(imgWithParam);
}

FileReader* ObjectCreator::createFileReader(ImgWithParam* imgWithParam)
{
	if(loadedFileType==BMP)
		return new BmpFileReader(imgWithParam);
	if(loadedFileType==XD)
		return new XdFileReader(imgWithParam);
}

Decompressor* ObjectCreator::createDecompressor(ImgWithParam* imgWithParam)
{
	if(loadedFileType==BMP)
		return new BmpDecompressor(imgWithParam);
	if(loadedFileType==XD)
		return new XdDecompressor(imgWithParam);
}

Converter* ObjectCreator::createConverter(ImgWithParam* imgWithParam, ColorMode colorMode)
{
	if(loadedFileType==BMP) //has loaded *.bmp file so it will be converted to xd extension
	{
		if(colorMode==DEDICATED)
			return new ToDedicatedPixmapConverter(imgWithParam);
		if(colorMode==GREY_SCALE)
			return new ToGreyScalePixmapConverter(imgWithParam);
		if(colorMode==IMPOSED)
			return new ToImposedPixmapConverter(imgWithParam);
	}
	if(loadedFileType==XD)
		return new XdToBmpConverter(imgWithParam, colorMode);
}
