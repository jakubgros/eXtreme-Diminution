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
	bool isBufferFull();
	size_t fillBuffer();
	void writeInBuffer(bool bit);
public:
	int getBytesProcessed() const {return bytesProcessed;}

	bool flush();
	void writeBit(bool bit);
	void write(unsigned char c);
	template <typename T> void write(T& data, size_t nOfBits);

	bool readBit(bool &bitRead); //returns false if eof, true if success. Read bit is assigned to bitRead
	unsigned char readByte(); //throws exception if there is not enough data
	template <typename toType> toType read(size_t nOfBits);
	std::bitset<64> readBitset(size_t nOfBits);

	void open(std::string filePath, char openMode);

	explicit FileBitStream(std::string filePath, char openMode);
	FileBitStream();
	~FileBitStream();
};

template <typename T>
void FileBitStream::write(T& data, size_t nOfBits)
{
	size_t counter=nOfBits;
	T tempData=data;
	unsigned char* tempBuff=(unsigned char*)(&tempData);
	const int BITS_IN_BYTE=8;
	int nOfBytes = std::ceil(nOfBits / 8.)-1;
	while(counter >0)
	{
		int shift=(nOfBytes*8-counter)%8;
		int bytesRead= std::ceil(counter / 8.)-1;
		bool bit = tempBuff[bytesRead]<<shift & mostSignif;
		writeBit(bit);
		--counter;
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

		temp<<=1;
		if(bit)
			temp|=1;
	}

	return temp;
}
