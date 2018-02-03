#include "FileBitStream.h"
#include <bitset>

size_t FileBitStream::fillBuffer()
{
	bitCounter=fread(&buffer, sizeof(unsigned char), 1, file);
	++bytesProcessed;
	return bitCounter;
}

bool FileBitStream::flush()
{
	if ((buffer | 0) == 0)
		return false;
	else
	{
		fwrite(&buffer, sizeof(unsigned char), sizeof(buffer), file);
		++bytesProcessed;
		return true;
	}
}

void FileBitStream::writeBit(bool bit)
{
	if(bitCounter==sizeof(buffer)*BITS_IN_BYTE)
		flush();
	else
	{
		buffer <<=1; //shift to make space
		if(bit==1)
			buffer |=1; //set bit
		++bitCounter;
	}
}

void FileBitStream::write(char c)
{

	int counter=sizeof(char)*BITS_IN_BYTE;
	while(counter>0)
	{
		int bit=c & 1;
		c>>=1;
		writeBit(bit);
		--counter;
	}
}

bool FileBitStream::readBit(bool &bitRead)
{
	if(bitCounter==0)
	{
		bitCounter=fillBuffer();
		if (bitCounter==0) return false; //eof
	}

	bool bitToReturn= (buffer & mostSignif) == mostSignif ? true: false;
	buffer <<=1; // shift
	--bitCounter;

	bitRead=bitToReturn;
	return true; //not eof
}

FileBitStream::FileBitStream()
{
	mostSignif|=1;
	mostSignif<<=sizeof(mostSignif)*BITS_IN_BYTE-1;
}

unsigned char FileBitStream::readByte()
{
	bool bit;
	unsigned char byte=0;
	for(int i=0; i<BITS_IN_BYTE; ++i)
	{
		if(readBit(bit)==false)
			throw std::exception("Not enough data to read a byte.");
		else
		{
			byte|=mostSignif;
			byte>>=1;
		}
	}
	return byte;
}

FileBitStream::FileBitStream(std::string filePath, char openMode)
{
	open(filePath, openMode);
}

void FileBitStream::open(std::string filePath, char mode)
{
	openMode=mode;
	buffer<<=sizeof(buffer)*BITS_IN_BYTE; //clean buffer
	bitCounter=0;

	char* fopenMode;
	if(openMode=='r')
		 fopenMode="rb";
	else if(openMode=='w')
		fopenMode="w+b";
	else
		throw std::invalid_argument("invalid openMode passed to fileBuffer constructor");

	fopen_s(&file, filePath.c_str(), fopenMode);

	if(file==nullptr)
		throw std::exception("can't open file");
}

FileBitStream::~FileBitStream()
{
	if(openMode=='w')
		flush();

	fclose(file);
}

std::bitset<64> FileBitStream::readBitset(size_t nOfBits)
{
	std::bitset<64> temp;

	temp.reset();

	for(int i=0; i<nOfBits; ++i)
	{
		bool bit;

		if(readBit(bit)==false)
			throw std::exception("not enough data in stream");

		if(bit)
			temp|=1;

		temp<<=1;
	}

	return temp;
}
