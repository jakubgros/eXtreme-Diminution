#include "Facade.h"
#include <iostream>

int main()
{
	Facade facade;
	try
	{
		facade.run();
	}
	catch(std::exception & error)
	{
		std::cout << error.what() <<std::endl;
		system("PAUSE");
		return 1;
	}

	system("PAUSE");
	return 0;
}
