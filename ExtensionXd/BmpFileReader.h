#pragma once
#include "FileReader.h"

struct SDL_Color;
typedef uint8_t Uint8;
struct SDL_Surface;

class BmpFileReader : public FileReader
{
public:
	explicit BmpFileReader(ImgWithParam* img);
	virtual ~BmpFileReader() = default;
	void read() override;
private:
	SDL_Surface* imgSurface;
private:
	void loadSurface();
	void readDimensions();
	void initPixmap();
	void readPixmap();
	Uint8* getPixelAddress(const int x,const int y);
	SDL_Color getPixel(const int x,const int y);
private:
	BmpFileReader(const BmpFileReader&);
	BmpFileReader& operator=(const BmpFileReader&);
};