#pragma once
#include <string>

class FileBitStream
{
	FILE* file;
	unsigned char buffer;
	int bitCounter;
	int bytesProcessed;
	char openMode;

	size_t fillBuffer();
public:
	int getBytesProcessed() const {return bytesProcessed;}

	bool flush();
	void writeBit(bool bit);
	void write(char c);
	template <typename T> void write(T& data, size_t nOfBits);
	bool readBit();
	explicit FileBitStream(std::string filePath, char openMode);
	FileBitStream()=default;
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
	}
}
