#pragma once
#include "SqliteDatabase.h"
#include "Game.h"
#include "Room.h"

class GameManager
{
public:
	GameManager(IDatabase*);
	GameManager() = default;
	~GameManager() = default;

	Game& createGame(Room&); 
	void deleteGame(int gameID);
	void submitQuestionsStatsToDB(std::vector<PlayerResults>& results);
	std::map<unsigned int, Game>& getGames() { return m_games; };



	bool doesGameExsist(int gameId);

private:
	IDatabase* m_database;
	std::map<unsigned int, Game> m_games;
};

