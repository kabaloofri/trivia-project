#pragma once
#include <vector>
#include <string>

class Buffer
{
public:
	Buffer();
	Buffer(std::string data) { setBuffer(data); }
	Buffer(char* data) { setBuffer(data); }
	
	std::vector<unsigned char> getBuffer() const;
	void setBuffer(std::string);
	void setBuffer(char*);
	std::string getMessage();

	std::string getStrBuffer() const;
private:
	std::vector<unsigned char> buffer;
};

