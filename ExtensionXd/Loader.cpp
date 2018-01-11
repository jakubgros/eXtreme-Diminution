#include "Loader.h"
#include "ObjectsCreator.h"
#include "Decompressor.h"
#include "Validator.h"
#include "FileReader.h"

Loader::Loader(ImgWithParam* img, ObjectCreator creator): img(img)
{
	decompressor=creator.createDecompressor(img);
	validator=creator.createValidator(img);
	fileReader=creator.createFileReader(img);
}

Loader::~Loader()
{
	delete decompressor;
	delete validator;
	delete fileReader;
}

bool Loader::load()
{
	fileReader->read();

	if(validator->validate()==false)
		return false;

	const Dictionary* dictionary=fileReader->getDictionary();
	decompressor->decompress(dictionary);

	return true;
}
