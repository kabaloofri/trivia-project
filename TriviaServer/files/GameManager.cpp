#include "GameManager.h"
#include <cmath>

GameManager::GameManager(IDatabase* DB) : m_database(DB)
{
	m_games = std::map<unsigned int, Game>();
}

Game& GameManager::createGame(Room& room)
{
	//get questions from DB
	auto questions = m_database->getQuestions(room.getRoomData().numOfQuestionsInGame);

	Game game = Game(questions, room.getAllUsers(), room.getRoomData().id, room.getRoomData().timePerQuestion); 
	m_games.emplace(room.getRoomData().id, game);
	
	
	return m_games[room.getRoomData().id];
}

void GameManager::deleteGame(int gameID)
{
	m_games.erase(gameID);
}


void GameManager::submitQuestionsStatsToDB(std::vector<PlayerResults>& results)
{
	for (auto& result : results)
	{
		m_database->submitGameStatistics(result, result.username);
	}
}

bool GameManager::doesGameExsist(int gameId)
{
	auto games = getGames();
	for (auto m : games)
	{
		if (m.first == gameId)
		{
			return true;
		}
	}
	return false;
}

