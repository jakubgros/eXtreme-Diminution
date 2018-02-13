#pragma once
#include "FileReader.h"
#include "types.h"
#include <SDL_pixels.h>

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
	SDL_Surface* imgSurface_;
private:
	void loadSurface();
	void checkResources() const;
	void readDimensions();
	void initPixmap();
	void readPixmap();
	SDL_Color getPixel(const int x, const int y) const;
	Uint8* getPixelAddress(const int x, const int y) const;
	void copyPixelFromAddress(Uint8* pixelAddress, SDL_Color& pixel) const;
private:
	BmpFileReader(const BmpFileReader&);
	BmpFileReader& operator=(const BmpFileReader&);
};