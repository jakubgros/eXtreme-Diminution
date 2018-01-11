#include "Compressor.h"
#include "types.h"

XdCompressor::~XdCompressor()
{
	delete dictionary;
}
