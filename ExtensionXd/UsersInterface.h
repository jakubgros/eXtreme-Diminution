#pragma once
#include <string>

enum ColorMode;

class UsersInterface
{
public:
	void printMsgToUser(std::string msg);
	std::string loadInputPath();
	std::string loadOuputPath();
	ColorMode loadColorMode();
};
