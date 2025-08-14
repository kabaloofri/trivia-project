#pragma once

#include "sqlite3.h"
#include <iostream>
#include "Question.h"
#include "Game.h"
#include "JsonRequestPacketSerializer.h"	

class IDatabase
{
public:
	IDatabase() = default;
	virtual ~IDatabase() = 0;

	virtual bool open() = 0;
	virtual bool close() = 0;
	virtual int doesUserExist(std::string) = 0;
	virtual int doesPasswordMatch(std::string, std::string) = 0;
	virtual int addNewUser(std::string, std::string, std::string) = 0;

	virtual std::vector<Question> getQuestions(int) = 0;

	virtual float getPLayerAverageAnswerTime(std::string) = 0;
	virtual int getNumOfCorrectAnswers(std::string) = 0;
	virtual int getNumOfTotalAnswers(std::string) = 0;
	virtual int getPlayerScore(std::string) = 0;
	virtual int getTotalGamesPlayed(std::string) = 0;

	virtual std::vector<std::string> getHighScores() = 0;

	virtual void submitGameStatistics(PlayerResults, std::string) = 0;

};