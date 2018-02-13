#include "BmpFileReader.h"
#include <SDL.h>

BmpFileReader::BmpFileReader(ImgWithParam* img) :
	FileReader(img),
	imgSurface_(nullptr)
{

}

void BmpFileReader::read()
{
	loadSurface();
	readDimensions();
	initPixmap();
	readPixmap();
}

void BmpFileReader::loadSurface()
{
	imgSurface_ = SDL_LoadBMP(img->inputFilepath.c_str());
	checkResources();
}

void BmpFileReader::checkResources() const
{
	if (imgSurface_ == nullptr)
		throw std::logic_error("BmpFileReader - cannot load BMP file");
}

void BmpFileReader::readDimensions()
{
	img->width = imgSurface_->w;
	img->height = imgSurface_->h;
}

void BmpFileReader::initPixmap()
{
	img->pixmap.resize(img->width);
	for (int i = 0; i < img->width; ++i)
		img->pixmap[i].resize(img->height);
}

void BmpFileReader::readPixmap()
{
	for (int x = 0; x < img->width; ++x)
		for (int y = 0; y < img->height; ++y)
		{
			SDL_Color newColor = getPixel(x, y);
			img->pixmap[x][y] = { newColor.r, newColor.g, newColor.b };
		}
}

SDL_Color BmpFileReader::getPixel(const int x, const int y) const
{
	Uint8* pixelAddress = getPixelAddress(x, y);
	SDL_Color pixel = {};
	if (pixelAddress)
		copyPixelFromAddress(pixelAddress, pixel);
	return pixel;
}

Uint8* BmpFileReader::getPixelAddress(const int x, const int y) const
{
	const int bpp = imgSurface_->format->BytesPerPixel;
	Uint8* pixelAddress = static_cast<Uint8*>(imgSurface_->pixels) + y * imgSurface_->pitch + x * bpp;
	return pixelAddress;
}

void BmpFileReader::copyPixelFromAddress(Uint8* pixelAddress, SDL_Color& pixel) const
{
	Uint32 rgbColor = 0;
	memcpy(&rgbColor, pixelAddress, imgSurface_->format->BytesPerPixel);
	SDL_GetRGB(rgbColor, imgSurface_->format, &pixel.r, &pixel.g, &pixel.b);
}