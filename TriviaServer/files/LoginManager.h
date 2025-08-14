#pragma once
#include "LoggedUser.h"
#include "sqliteDatabase.h"
#include "JsonRequestPacketSerializer.h"

class LoginManager
{
private:
	IDatabase* m_database;
	std::vector <LoggedUser> m_loggedUsers;
public:
	LoginManager(IDatabase* database);
	LoginManager() = default;
	~LoginManager() = default;

	bool signup(std::string name, std::string password, std::string mail);
	bool login(std::string name, std::string password);	
	void logout(std::string);

};