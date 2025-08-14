#pragma once
#include "LoggedUser.h"
#include <vector>
#include "json.hpp"
struct RoomData
{
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int currentAmountOfPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	unsigned int status;

	//for easy serialziation
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(RoomData, id, name, maxPlayers, currentAmountOfPlayers, numOfQuestionsInGame, timePerQuestion, status);
};


enum //room states
{
	state_waitingForPlayers,
	state_begginingGame,
	state_gameStarted,
	state_inactive
};

class Room
{
public:
	Room(RoomData, LoggedUser);
	Room() = default;
	
	bool addUser(LoggedUser);
	void removeUser(LoggedUser);
	std::vector<std::string> getAllUsers();
	RoomData getRoomData();
	
	void startGame();
	void setStatus(unsigned int status) { m_metadata.status = status; };
	void changeSettings(int questionsCount, int timePerQuestion);

	double getGameStartTime() const { return m_timeUntilGameStarts; };

private:
	void startGameThread();

	RoomData m_metadata;
	std::vector<LoggedUser>	m_users;
	float m_timeUntilGameStarts;
};