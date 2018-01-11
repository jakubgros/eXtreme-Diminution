#pragma once
#include "Compressor.h"


class ObjectCreator;
class ImgWithParam;
class Decompressor;
class Validator;
class FileReader;

class Loader
{
	ImgWithParam* img;
	Decompressor* decompressor;
	Validator* validator;
	FileReader* fileReader;

public:
	Loader(ImgWithParam* img, ObjectCreator creator);
	~Loader();
	bool load();
};
