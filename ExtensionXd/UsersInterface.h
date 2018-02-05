#pragma once
#include <string>

enum ColorMode;

class UsersInterface
{
public:
	explicit UsersInterface();
	void printMsgToUser(std::string msg);
	std::string loadInputPath();
	std::string loadOuputPath();
	ColorMode loadColorMode();
	std::string getInputExtension();
private:
	std::string inputPath;
	unsigned int colorMode;
	bool isInputLoaded;
private:
	void displayColorModeMenu();
};
