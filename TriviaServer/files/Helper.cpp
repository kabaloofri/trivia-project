#include "Helper.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using std::string;

// recieves the type code of the message from socket (3 bytes)
// and returns the code. if no message found in the socket returns 0 (which means the client disconnected)
int Helper::getMessageTypeCode(SOCKET sc)
{
	string s = getPartFromSocket(sc, 3);
	string msg(s);

	if (msg == "")
		return 0;

	int res = std::atoi(s.c_str());
	return  res;
}


void Helper::send_update_message_to_client(SOCKET sc, const string& file_content, const string& second_username, const string &all_users)
{
	//TRACE("all users: %s\n", all_users.c_str())
	const string code = std::to_string(MT_SERVER_UPDATE);
	const string current_file_size = getPaddedNumber(file_content.size(), 5);
	const string username_size = getPaddedNumber(second_username.size(), 2);
	const string all_users_size = getPaddedNumber(all_users.size(), 5);
	const string res = code + current_file_size + file_content + username_size + second_username + all_users_size + all_users;
	//TRACE("message: %s\n", res.c_str());
	sendData(sc, res);
}

// recieve data from socket according byteSize
// returns the data as int
int Helper::getIntPartFromSocket(SOCKET sc, int bytesNum)
{
	string s = getPartFromSocket(sc, bytesNum, 0);
	return atoi(s.c_str());
}

// recieve data from socket according byteSize
// returns the data as string
string Helper::getStringPartFromSocket(SOCKET sc, int bytesNum)
{
	string s = getPartFromSocket(sc, bytesNum, 0);
	return s;
}

// return string after padding zeros if necessary
string Helper::getPaddedNumber(int num, int digits)
{
	std::ostringstream ostr;
	ostr << std::setw(digits) << std::setfill('0') << num;
	return ostr.str();

}

// recieve data from socket according byteSize
char* Helper::getPartFromSocket(SOCKET sc, int bytesNum)
{
	return getPartFromSocket(sc, bytesNum, 0);
}

char* Helper::getPartFromSocket(SOCKET sc, int bytesNum, int flags)
{
	if (bytesNum == 0)
	{
		return nullptr;
	}

	
	char* buff = new char[bytesNum + 1];
	int res = recv(sc, buff, bytesNum, flags);
	
	buff[bytesNum] = '\0';

	if (res == INVALID_SOCKET)
	{
		std::string s = "Error while recieving from socket: ";
		s += std::to_string(sc);
		throw std::exception(s.c_str());
	}
	return buff;
}

// send data to socket
// this is private function
void Helper::sendData(SOCKET sc, std::string message)
{
	string data = message;

	if (send(sc, data.c_str(), message.size(), 0) == INVALID_SOCKET)
	{
		throw std::exception("Error while sending message to client");
	}
}