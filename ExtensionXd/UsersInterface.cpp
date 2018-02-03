#include "UsersInterface.h"
#include "Types.h"
#include <iostream>

void UsersInterface::printMsgToUser(std::string msg)
{
	std::cout << msg;
}

std::string UsersInterface::loadInputPath()
{
	std::cout << "Wprowadz sciezke do pliku wejsciowego: ";
	std::string path;
	std::cin >> path;
	return path;
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