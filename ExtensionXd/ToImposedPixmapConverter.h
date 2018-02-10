#pragma once
#include "Converter.h"

struct Rgb;

class ToImposedPixmapConverter : public BmpToXdConverter
{
public:
	explicit ToImposedPixmapConverter(ImgWithParam* imgWithParam);
	~ToImposedPixmapConverter() = default;
	void convert();
private:
	void checkResources() const;
	ToImposedPixmapConverter(const ToImposedPixmapConverter&);
	ToImposedPixmapConverter& operator=(const ToImposedPixmapConverter&);
};