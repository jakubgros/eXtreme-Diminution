#pragma once
#include <string>
#include <bitset>

class FileBitStream
{
	const int BITS_IN_BYTE=8;
	FILE* file;
	unsigned char buffer;
	int bitCounter;
	int bytesProcessed;
	char openMode;
	unsigned char mostSignif;

	size_t fillBuffer();
public:
	int getBytesProcessed() const {return bytesProcessed;}

	bool flush();
	void writeBit(bool bit);
	void write(char c);
	template <typename T> void write(T& data, size_t nOfBits);
	bool readBit(bool &bitRead); //returns false if eof, true if success. Read bit is assigned to bitRead
	unsigned char readByte(); //throws exception if there is not enough data
	template <typename toType> toType read(size_t nOfBits);
	std::bitset<64> readBitset(size_t nOfBits);
	explicit FileBitStream(std::string filePath, char openMode);
	FileBitStream();
	void open(std::string filePath, char openMode);
	~FileBitStream();
};

template <typename T>
void FileBitStream::write(T& data, size_t nOfBits)
{
	size_t counter=0;
	unsigned char* tempBuff=(unsigned char*)(&data);
	const int BITS_IN_BYTE=8;

	while(counter<nOfBits)
	{
		int bit= tempBuff[counter%BITS_IN_BYTE]>>counter & 1;
		writeBit(bit);
		++counter;
	}
}

template <typename toType>
toType FileBitStream::read(size_t nOfBits)
{
	toType temp=0;

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
