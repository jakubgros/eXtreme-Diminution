#include "Facade.h"
#include "Loader.h"
#include "Converter.h"
#include "Types.h"
#include "Saver.h"
#include "Display.h"

void Facade::fillImposedPalettes()
{
	int i = 0;
	for (int R = 0; R <= 255; R += 85)
		for (int G = 0; G <= 255; G += 85)
			for (int B = 0; B <= 255; B += 85)
			{
				img->imposedColorPalette[i] = { R, G, B };
				++i;
			}



	i = 0;
	for (int j = 0; j < 255; j += 4)
	{
		img->imposedGreyPalette[i] = { j, j, j };
		++i;
	}

}

void Facade::getInfoFromUser()
{
	img=new ImgWithParam;
	fillImposedPalettes();
	usersInterface=new UsersInterface;
	img->inputFilepath=usersInterface->loadInputPath();
	img->outputFilepath=usersInterface->loadOuputPath();
	img->colorMode=usersInterface->loadColorMode();
	colorMode=new ColorMode(img->colorMode);
	img->fileType=usersInterface->getInputExtension();

	
}

void Facade::makeCreator()
{
	std::string extension=img->fileType;
	if(extension=="bmp")
		creator=new ObjectCreator(FamilyType::BMP);
	else if(extension=="xd")
		creator=new ObjectCreator(FamilyType::XD);
	else
	{
		std::string* msg=new std::string(extension+" is unsupported file extension");
		throw std::exception(msg->c_str());
	}
}

void Facade::load()
{
	loader=new Loader(img, *creator);
	if(loader->load()==false)
		throw std::exception("Input file is corrupted");
}

void Facade::convert()
{
	converter=creator->createConverter(img, *colorMode);
	converter->convert();
}

void Facade::printImg()
{
	display=new Display(img);
	display->print();
}

void Facade::save()
{
	saver=new Saver(img, *creator);

	saver->save();
	//TODO: nie wiem dlaczego ale zwraca false gdzies jak juz istnieje plik o podanej nazwie (a nie implementowalismy tego)
	/*
	while(saver->save()==false);
	{
		usersInterface->printMsgToUser("Istnieje juz plik o takiej nazwie. Prosze podac inna nazwe lub sciezke. \n");
		img->outputFilepath=usersInterface->loadOuputPath();	
	}
	*/
}

void Facade::run()
{
	getInfoFromUser();
	makeCreator();
	load();
	convert();
	if(img->fileType=="xd")
		printImg();
	save();
}



Facade::~Facade()
{
	delete img;
	delete creator;
	delete colorMode;
	delete usersInterface;
	delete converter;
	delete display;
	delete loader;
	delete saver;
}
