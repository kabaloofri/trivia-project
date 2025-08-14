#include "LoginManager.h"

LoginManager::LoginManager(IDatabase* database)
{
    m_database = database;
}

bool LoginManager::signup(std::string name, std::string password, std::string mail)
{
    if (m_database->doesUserExist(name))
    {
        return false;
    }
    m_database->addNewUser(name, password, mail);
    return true;

}

bool LoginManager::login(std::string name, std::string password)
{
    if (m_database->doesPasswordMatch(password, name))
    {
        //add the user to the logged users
        LoggedUser user(name);
        for (auto& user : m_loggedUsers)
        {
            if (user.getUsername() == name)
            {
                return false; //player is already logged in
            }
        }
        m_loggedUsers.push_back(user);
        
        //finish
        return true;
    }
    else
    {
        return false;
    }
}

void LoginManager::logout(std::string name)
{
    for (auto i = m_loggedUsers.begin(); m_loggedUsers.end() != i; i++)
    {
        if (i->getUsername() == name)
        {
            m_loggedUsers.erase(i);
            return;
        }
    }
}
