#include "StatisticsManager.h"

StatisticsManager::StatisticsManager(IDatabase* DB)
{
	m_database = DB;
}


std::vector<std::string> StatisticsManager::getHighScore()
{
	auto highScores = m_database->getHighScores();
	return highScores;
}

std::vector<std::string> StatisticsManager::getUserStatistics(std::string user)
{
	auto stats = std::vector<std::string>();
	stats.push_back(std::to_string(m_database->getPLayerAverageAnswerTime(user)));
	stats.push_back(std::to_string(m_database->getNumOfCorrectAnswers(user)));
	stats.push_back(std::to_string(m_database->getNumOfTotalAnswers(user)));
	stats.push_back(std::to_string(m_database->getPlayerScore(user)));
	stats.push_back(std::to_string(m_database->getTotalGamesPlayed(user)));

	return stats;
}
