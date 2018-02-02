#pragma once
#include "Types.h"

struct ImgWithParam;

class Display
{
	ImgWithParam* img;
public:
	Display(ImgWithParam* img): img(img){};
	void print(); //TODO: implement
};
