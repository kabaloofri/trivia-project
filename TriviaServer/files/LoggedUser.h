#pragma once
#include <string>

class LoggedUser
{
public:
	LoggedUser(std::string username)
	{
		m_username = username;
	}
	LoggedUser() = default;

	std::string getUsername() const
	{
		return m_username;
	}

	bool operator<(LoggedUser other)
	{
		return m_username < other.m_username;
	}
private:
	std::string m_username;
	
};
