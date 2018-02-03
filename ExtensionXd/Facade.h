#pragma once
#include "UsersInterface.h"
#include "Display.h"
#include "ObjectsCreator.h"
#include "Loader.h"
#include "Saver.h"

class Converter;
struct ImgWithParam;
enum ColorMode;

class Facade
{	
	ImgWithParam* img;
	ObjectCreator* creator;
	ColorMode* colorMode;

	UsersInterface* usersInterface;
	Converter* converter;
	Display* display;
	Loader* loader;
	Saver* saver;

public:
	void fillImposedPalettes();
	void getInfoFromUser();
	void makeCreator();
	void load();
	void convert();
	void printImg();
	void save();
	void run();
	Facade()=default;
	~Facade();
};
