#include "Facade.h"
#include "Loader.h"
#include "Converter.h"
#include "types.h"
#include "Saver.h"
#include "Display.h"

void Facade::getInfoFromUser()
{
	img=new ImgWithParam;
	usersInterface=new UsersInterface;
	img->inputFilepath=usersInterface->loadInputPath();
	img->outputFilepath=usersInterface->loadOuputPath();
	img->colorMode=usersInterface->loadColorMode();
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
	while(saver->save()==false);
	{
		usersInterface->printMsgToUser("Istnieje juz plik o takiej nazwie. Prosze podac inna nazwe lub sciezke. \n");
		img->outputFilepath=usersInterface->loadOuputPath();	
	}
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
