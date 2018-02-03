#include "BmpFileReader.h"
#include <SDL.h>
#include "Types.h"
#include <iostream>

BmpFileReader::BmpFileReader(ImgWithParam* img) :
	FileReader(img),
	imgSurface(nullptr)
{

}

Uint8* BmpFileReader::getPixelAddress(const int x, const int y)
{
	/* Pobieramy informacji ile bajtów zajmuje jeden pixel */
	const int bpp = imgSurface->format->BytesPerPixel;
	/* Obliczamy adres pixela */
	return (Uint8*)imgSurface->pixels + y * imgSurface->pitch + x * bpp;
}

SDL_Color BmpFileReader::getPixel(const int x, const int y)
{
	Uint8* pixelAddress = getPixelAddress(x, y);
	SDL_Color color = {};
	if (pixelAddress)
	{
		Uint32 rgbColor = 0;
		memcpy(&rgbColor, pixelAddress, imgSurface->format->BytesPerPixel);
		SDL_GetRGB(rgbColor, imgSurface->format, &color.r, &color.g, &color.b);
	}
	return color;
}

void BmpFileReader::loadSurface()
{
	imgSurface = SDL_LoadBMP(img->inputFilepath.c_str());
	if (imgSurface == nullptr)
	{
		std::string* msg = new std::string("Cannot load BMP file");
		throw std::exception(msg->c_str());
	}
}

void BmpFileReader::readDimensions()
{
	img->width = imgSurface->w;
	img->height = imgSurface->h;
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

void BmpFileReader::read()
{
	loadSurface();
	readDimensions();
	initPixmap();
	readPixmap();
}