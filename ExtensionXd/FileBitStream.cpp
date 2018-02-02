#include "FileBitStream.h"


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
	if(bitCounter==sizeof(buffer))
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
	int counter=sizeof(char);
	while(counter>0)
	{
		int bit=c & 1;
		c>>=1;
		writeBit(bit);
	}
}


bool FileBitStream::readBit()
{
	if(bitCounter==0)
	{
		bitCounter=fillBuffer();
		if (bitCounter==0) return false; //eof
	}

	bool bitToReturn= (buffer & 1) == 1 ? true: false;
	buffer >>=1; // shift
	--bitCounter;
	return bitToReturn;
}

FileBitStream::FileBitStream(std::string filePath, char openMode)
{
	open(filePath, openMode);
}

void FileBitStream::open(std::string filePath, char mode)
{
	openMode=mode;
	buffer<<=sizeof(buffer); //clean buffer
	bitCounter=0;

	char* fopenMode;
	if(openMode=='r')
		 fopenMode="rb";
	else if(openMode=='w')
		fopenMode="w+b";
	else
		throw std::invalid_argument("invalid openMode passed to fileBuffer constructor");

	file=fopen(filePath.c_str(), fopenMode);

	if(file==nullptr)
		throw std::exception("can't open file");
}

FileBitStream::~FileBitStream()
{
	if(openMode=='w')
		flush();

	fclose(file);
}

