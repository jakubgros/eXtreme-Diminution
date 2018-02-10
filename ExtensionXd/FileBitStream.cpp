#include "FileBitStream.h"
#include <bitset>
#include <iostream>


void FileBitStream::write(unsigned char c)
{
	int bitsToWrite=sizeof(unsigned char)*BITS_IN_BYTE;

	for(int i=0; i<bitsToWrite; ++i)
	{
		bool bit=c & mostSignif;
		writeBit(bit);
		c<<=1;
	}
}

void FileBitStream::writeBit(bool bit)
{
	if(isBufferFull())
		flush();

	writeInBuffer(bit);
}

bool FileBitStream::isBufferFull()
{
	return bitCounter == sizeof(buffer) * BITS_IN_BYTE;
}

bool FileBitStream::flush()
{
	if (bitCounter==0)
		return false;
	else
	{
		fwrite(&buffer, sizeof(unsigned char), 1, file);
		++bytesProcessed;
		bitCounter=0;
		return true;
	}
}

void FileBitStream::writeInBuffer(bool bit)
{
	buffer <<=1; //shift to make space
	if(bit==1)
		buffer |=1; //set bit, default set to 0
	++bitCounter;
}




bool FileBitStream::readBit(bool &bitRead)
{
	if(bitCounter==0)
	{
		bitCounter=fillBuffer();
		if (bitCounter==0)
			return false; //eof
	}

	bool bitToReturn= (buffer & mostSignif) == mostSignif ? true: false;
	buffer <<=1; // shift
	--bitCounter;

	bitRead=bitToReturn;
	return true; //not eof
}

unsigned char FileBitStream::readByte()
{
	bool bit;
	unsigned char byte=0;
	for(int i=0; i<BITS_IN_BYTE; ++i)
	{
		if(readBit(bit)==false)
			throw std::exception("Not enough data to read a byte. ");
		else
		{
			byte<<=1;

			if(bit==true)
				byte|=1;
		}
	}
	return byte;
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

		temp<<=1;
		if(bit==true)
			temp|=1;
	}

	return temp;
}

size_t FileBitStream::fillBuffer()
{
	bitCounter=fread(&buffer, sizeof(unsigned char), 1, file)*BITS_IN_BYTE;
	++bytesProcessed;
	return bitCounter;
}




FileBitStream::FileBitStream(std::string filePath, char openMode)
{
	open(filePath, openMode);
}

void FileBitStream::open(std::string filePath, char mode)
{
	openMode=mode;
	buffer=0; //clean buffer
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

FileBitStream::FileBitStream()
{
	mostSignif|=1;
	mostSignif<<=sizeof(mostSignif)*BITS_IN_BYTE-1;
}