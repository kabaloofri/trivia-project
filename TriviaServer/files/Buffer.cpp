#include "Buffer.h"

#include <bitset>
#include <iostream>
#include <stdio.h>

Buffer::Buffer()
{
	buffer = std::vector<unsigned char>();
}

std::vector<unsigned char> Buffer::getBuffer() const
{
	return buffer;
}

// takes string of "101010", and puts into buffer - 001010
void Buffer::setBuffer(std::string msg)
{
	buffer.assign(msg.begin(), msg.end());
}

void Buffer::setBuffer(char* data)
{
	std::vector<unsigned char> buff;
	for (size_t i = 0; i < strlen(data); i++)
	{
		buff.push_back(data[i]);
	}
	delete data;
	buffer = buff;
}

//get the ascii thing of the buffer "hello"
std::string Buffer::getMessage()
{
	std::string str;
	str.assign(buffer.begin(), buffer.end());
	return str;
}

//get the buffer in string format "010101"
std::string Buffer::getStrBuffer() const
{
	return std::string(buffer.begin(), buffer.end());
}
