#include "UsersInterface.h"
#include "Types.h"
#include <iostream>

UsersInterface::UsersInterface() :
	inputPath(""),
	colorMode(0),
	isInputLoaded(false)
	
{
	
}

void UsersInterface::printMsgToUser(std::string msg)
{
	std::cout << msg;
}

std::string UsersInterface::loadInputPath()
{
	std::cout << "Wprowadz sciezke do pliku wejsciowego: ";
	//std::cin >> inputPath;
	inputPath="D:\\b.bmp"; //TODO: usunac to i odkomentowac powyzsze
	isInputLoaded = true;
	return inputPath;
}

std::string UsersInterface::loadOuputPath()
{
	std::cout << "Wprowadz sciezke do pliku wyjsciowego: ";
	std::string path;
	//std::cin >> path;
	path="D:\\b.xd"; //TODO: usunac to i odkomentowac powyzsze
	return path;
}


ColorMode UsersInterface::loadColorMode()
{
	if (isInputLoaded)
	{
		if (getInputExtension() == "bmp")
			displayColorModeMenu();
	}
	else
	{
		displayColorModeMenu();
		//std::cin >> colorMode;
		colorMode=0; //TODO: usunac to i odkomentowac powyzsze
	}

	return ColorMode(colorMode);
}

void UsersInterface::displayColorModeMenu()
{
	std::cout << "[0] skala szarości" << std::endl
		<< "[1] dedykowana paleta" << std::endl
		<< "[2] narzucona paleta" << std::endl
		<< "Wybierz palete (podaj numer):";

}


std::string UsersInterface::getInputExtension()
{
	std::string inputExtension;
	if (isInputLoaded)
	{
		for (size_t i = inputPath.size() - 1; i >= 0; --i)
			if (inputPath[i] != '.')
				inputExtension += inputPath[i];
			else
				break;
		std::reverse(inputExtension.begin(), inputExtension.end());
	}
	return inputExtension;
}

