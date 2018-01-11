#pragma once

struct ImgWithParam;
class ObjectCreator;
class FileWriter;
class Compressor;

class Saver
{
	FileWriter* fileWriter;
	Compressor* compressor;
	ImgWithParam* img;
public:
	Saver(ImgWithParam* img, ObjectCreator creator);
	~Saver();
	bool save();
};