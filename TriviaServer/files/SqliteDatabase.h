#pragma once
#include "Idatabase.h"

class SqliteDatabase : public IDatabase
{
public:
	SqliteDatabase();
	virtual ~SqliteDatabase();

	void CreateQuestions(int numOfQuestions);
	bool execCommand(std::string& command, int (*callback)(void*, int, char**, char**), void* arg1);

	int doesUserExist(std::string) override;
	int doesPasswordMatch(std::string, std::string) override;
	int addNewUser(std::string, std::string, std::string) override;

	std::vector<Question> getQuestions(int) override;

	float getPLayerAverageAnswerTime(std::string) override;
	int getNumOfCorrectAnswers(std::string) override;
	int getNumOfTotalAnswers(std::string) override;
	int getPlayerScore(std::string) override;
	int getTotalGamesPlayed(std::string) override;

	std::vector<std::string> getHighScores() override;

	void submitGameStatistics(PlayerResults data, std::string name) override;

private:
	bool open() override;
	bool close() override;
	
	static int callback(void* data, int argc, char** argv, char** azColName);
	static int getQuestionsCallback(void* vectorRef, int count, char** data, char** azColName);
	static int getFloatCallback(void* floatRef, int count, char** data, char** azColName);
	
	sqlite3* m_DB;
};