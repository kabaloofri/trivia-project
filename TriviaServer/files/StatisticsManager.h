#pragma once
#include "SqliteDatabase.h"


class StatisticsManager
{
public:
	StatisticsManager(IDatabase*);
	StatisticsManager() = default;

	std::vector<std::string> getHighScore();
	std::vector<std::string> getUserStatistics(std::string);

private:
	IDatabase* m_database;
};