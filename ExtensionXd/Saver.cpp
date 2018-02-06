#include "Saver.h"
#include "ObjectsCreator.h"
#include "Compressor.h"
#include "FileWriter.h"

Saver::Saver(ImgWithParam* img, ObjectCreator creator): img(img)
{
	fileWriter=creator.createFileWriter(img);
	compressor=creator.createCompressor(img);
}

Saver::~Saver()
{
	delete fileWriter;
	delete compressor;
}

bool Saver::save()
{
	Dictionary* dictionary=compressor->compress();
	fileWriter->setDictionaryToSave(dictionary);
	return fileWriter->write();
}

//bug: dla obrazka 5x5 całego czarnego w słowniku jest jeden element, dla którego codeWord jest pusty