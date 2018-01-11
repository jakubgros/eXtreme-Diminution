#pragma once
#include "types.h"

struct ImgWithParam;

class Display
{
	ImgWithParam* img;
public:
	Display(ImgWithParam* img): img(img){};
	void print(); //TODO: implement
};
