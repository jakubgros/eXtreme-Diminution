#include "ToImposedPixmapConverter.h"
#include "Types.h"
#include "DitheringFS.h"

ToImposedPixmapConverter::ToImposedPixmapConverter(ImgWithParam* imgWithParam) :
	BmpToXdConverter(imgWithParam)
{

}

void ToImposedPixmapConverter::convert()
{
	checkResources();
	dfs::ditheringFs(&img->pixmap, &img->imposedColorPalette);
}


void ToImposedPixmapConverter::checkResources() const
{
	if (img == nullptr)
		throw std::logic_error("ToDedicatedPixmapConverter - nullptr resources");
}
