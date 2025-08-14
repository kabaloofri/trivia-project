#include "Room.h"

#include <thread>
#include <chrono>

Room::Room(RoomData roomData, LoggedUser loggedUser)
{
	m_metadata = roomData;
	m_users.push_back(loggedUser); //set admin as first user
	m_timeUntilGameStarts = -1; //if game isnt starting then this is -1
}

bool Room::addUser(LoggedUser user)
{
	if (m_users.size() >= m_metadata.maxPlayers)
	{
		return false; //false if the room is full
	}
	m_users.push_back(user);
	m_metadata.currentAmountOfPlayers++;
	return true;
}

void Room::removeUser(LoggedUser user)
{
	for (auto i = m_users.begin(); i != m_users.end(); i++)
	{
		if (i->getUsername() == user.getUsername())
		{
			m_users.erase(i);
			m_metadata.currentAmountOfPlayers--;
			return;
		}
	}
}

std::vector<std::string> Room::getAllUsers()
{
	std::vector<std::string> users;
	for (auto i = m_users.begin(); i != m_users.end(); i++)
	{
		users.push_back(i->getUsername()); //first user should be admin
	}
	return users;
}

RoomData Room::getRoomData()
{
	return m_metadata;
}

void Room::startGame()
{
	setStatus(state_begginingGame);

	m_timeUntilGameStarts = 10;

	std::thread t = std::thread(&Room::startGameThread, this);
	t.detach();
}

void Room::changeSettings(int questionsCount, int timePerQuestion)
{
	m_metadata.numOfQuestionsInGame = questionsCount;
	m_metadata.timePerQuestion = timePerQuestion;
}

void Room::startGameThread()
{
	int time = 0.1;
	while (m_timeUntilGameStarts >= 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(time * 1000)); //sleep for 0.1 seconds
		m_timeUntilGameStarts -= time;
	}
	setStatus(state_gameStarted); //set the status that the game is running
}


