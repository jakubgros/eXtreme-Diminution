#include "UsersInterface.h"
#include "Types.h"
#include <iostream>

UsersInterface::UsersInterface() :
	inputPath("")
{
	
}

void UsersInterface::printMsgToUser(std::string msg)
{
	std::cout << msg;
}

std::string UsersInterface::loadInputPath()
{
	std::cout << "Wprowadz sciezke do pliku wejsciowego: ";
	std::cin >> inputPath;
	return inputPath;
}

std::string UsersInterface::loadOuputPath()
{
	std::cout << "Wprowadz sciezke do pliku wyjsciowego: ";
	std::string path;
	std::cin >> path;
	return path;
}


ColorMode UsersInterface::loadColorMode()
{
	std::cout 	<< "[0] skala szarości" << std::endl
	<< "[1] dedykowana paleta" << std::endl
	<< "[2] narzucona paleta" << std::endl
	<< "Wybierz palete (podaj numer):";
	unsigned int mode;
	std::cin >> mode;
	return ColorMode(mode);
}

std::string UsersInterface::getInputExtension()
{
	std::string extension = "";
	for (int i = inputPath.size() - 1; i >= 0; --i)
	{
		if (inputPath[i] != '.')
			extension += inputPath[i];
		else
			break;
	}
	std::reverse(extension.begin(), extension.end());
	return extension;
}