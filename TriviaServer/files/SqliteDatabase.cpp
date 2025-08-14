#include "SqliteDatabase.h"
#include <io.h>
#include <fstream>
#include <streambuf>

#include "json.hpp"

std::string dbFileName = "database/database.sqlite";

//mini function to check weather there is something in the questions table
int doQuestionsExist(void* boolRef, int argc, char** argv, char** azColName)
{
	auto doesExsist = (bool*)boolRef;
	*doesExsist = true;
	return 0;
}


SqliteDatabase::SqliteDatabase()
{
	if (!open())
	{
		std::cout << "error opening database!";
		return;
	}
	//check if there are already questions in the list
	bool questionsExist = false;
	std::string query = "SELECT * FROM Questions LIMIT 1;";
	execCommand(query, doQuestionsExist, &questionsExist);

	if (questionsExist)
	{
		return;
	}
	//if there are no questions, create 50 (should only happen first time)
	CreateQuestions(50);
}

SqliteDatabase::~SqliteDatabase()
{
	close();
}



void SqliteDatabase::CreateQuestions(int numOfQuestions)
{
	//get the questions
	std::string FILE_NAME = "questions.txt";

	std::string command = "curl \"https://opentdb.com/api.php?amount=" + std::to_string(numOfQuestions) + "&type=multiple\" -o " + FILE_NAME;
	system(command.c_str());
	std::ifstream file(FILE_NAME);
	std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();


	//delete the file
	command = "del " + FILE_NAME;
	system(command.c_str());

	//parse the questions
	nlohmann::json jsn = nlohmann::json::parse(data);
	std::vector<Question> questions;

	for (auto item : jsn["results"])
	{
		std::string question = item["question"];
		std::string answer = item["correct_answer"];
		std::vector<std::string> otherAnswers = item["incorrect_answers"];

		Question Q = Question(question, answer, otherAnswers);
		questions.push_back(Q);
	}

	//add to database
	for (auto question : questions)
	{
		std::vector<std::string> other = question.getPossibleAnswers();
		
		std::string query = "INSERT INTO Questions (question, Correct_Answer, Answer_1, Answer_2, Answer_3) "
			"Values ('" + question.getQuestion() + "', '" + question.getPossibleAnswers()[question.getCorrectAnswerID()] + "', "
			"'" + other[0] + "', '" + other[1] + "', '" + other[2] + "');";

		execCommand(query, NULL, NULL);
	}

}





// safe sqlite command executor
bool SqliteDatabase::execCommand(std::string& command, int (*callback)(void*, int, char**, char**), void* arg1)
{
	char** errorMsg = nullptr;
	int res = sqlite3_exec(m_DB, command.c_str(), callback, arg1, errorMsg);

	if (res == SQLITE_ERROR)
	{
		std::cout << "error while executing sql command: " << command << "\n error was: " << errorMsg << " and function returned " << res << "\n";
		sqlite3_free(errorMsg);
		return false;
	}
	return true;
}

bool SqliteDatabase::open()
{
	//check if exsist
	if (_access(dbFileName.c_str(), 0) != 0)
	{
		//create new file
		sqlite3_open(dbFileName.c_str(), &m_DB);

		//if not create the table
		std::string msg = "CREATE TABLE USERS (NAME TEXT PRIMARY KEY NOT NULL,PASSWORD TEXT NOT NULL , MAIL TEXT NOT NULL);";
		execCommand(msg, nullptr, nullptr);
		msg = "CREATE TABLE Questions (Question TEXT PRIMARY KEY NOT NULL, Correct_Answer TEXT NOT NULL, Answer_1 TEXT NOT NULL, Answer_2 TEXT NOT NULL, Answer_3 TEXT NOT NULL);";
		execCommand(msg, nullptr, nullptr);
		msg = "CREATE TABLE Statistics (Username TEXT PRIMARY KEY NOT NULL, AverageAnswerTime FLOAT, numOfCorrectAnswers INT,TotalAnswers INT, playedGames INT, Score INT, FOREIGN KEY(username) REFERENCES Users(name));";
		execCommand(msg, nullptr, nullptr);
		
	}
	else
	{
		int res = sqlite3_open(dbFileName.c_str(), &m_DB);

		auto stuff = (dbFileName);
		//check if open
		if (res != SQLITE_OK)
		{
			std::cout << "cannot open the data base";
			m_DB = nullptr;
			return false;
		}
	}
	

	//if valid
	return true;
}

bool SqliteDatabase::close()
{
	sqlite3_close(m_DB);
	m_DB = nullptr;
	return true;
}

int SqliteDatabase::doesUserExist(std::string name)
{
	std::vector<std::string> vec;
	std::string msg = "SELECT * FROM USERS WHERE NAME LIKE '" + name + "';";
	execCommand(msg, callback, &vec);

	//if the user not exsist
	if (vec.size() == 0)
	{
		return 0;
	}

	//if he exsist
	else
	{
		return 1;
	}
}

int SqliteDatabase::doesPasswordMatch(std::string password, std::string name)
{
	std::vector<std::string> vec;
	std::string msg = "SELECT * FROM USERS WHERE PASSWORD LIKE '" + password + "' AND NAME LIKE '" + name + "';";
	execCommand(msg, callback, &vec);

	//if the user not exsist
	if (vec.size() == 0)
	{
		return 0;
	}

	//if he exsist
	else
	{
		return 1;
	}
}

int SqliteDatabase::addNewUser(std::string name, std::string password, std::string mail)
{
	//maybe need to check if username exsist
	std::string msg = "INSERT INTO USERS (NAME , PASSWORD , MAIL) VALUES ('" + name + "','" + password + "','" + mail + "');";
	execCommand(msg, nullptr, nullptr);

	std::cout << "adding user: " << name << ", " << password << ", " << mail << "\n";
	//add empty stats
	msg = "INSERT INTO Statistics (Username, AverageAnswerTime, numOfCorrectAnswers,TotalAnswers, playedGames, Score)"
		"VALUES ('" + name + "', " + std::to_string(0) + ", " + std::to_string(0) + ", " + std::to_string(0) +
		", " + std::to_string(0) + ", " + std::to_string(0) + ");";
	execCommand(msg, nullptr, nullptr);
	return true;
}

std::vector<Question> SqliteDatabase::getQuestions(int amount)
{
	std::vector<Question>* questions = new std::vector<Question>();
	std::string query = "SELECT * FROM Questions ORDER BY RANDOM() LIMIT " + std::to_string(amount) + ";";
	//sort by random and get top <amount>
	execCommand(query, getQuestionsCallback, questions);


	//delete the questions to send from the db
	for (auto question : *questions)
	{
		query = "DELETE FROM QUESTIONS WHERE Question LIKE '" + question.getQuestion() + "';";
		execCommand(query, nullptr, nullptr);
	}
	CreateQuestions(amount); //place back new questions
	return *questions;
}

float SqliteDatabase::getPLayerAverageAnswerTime(std::string username)
{
	std::string query = "SELECT averageAnswerTime FROM Statistics WHERE username LIKE '" + username + "';";
	float avrgAnsTime;
	execCommand(query, getFloatCallback, &avrgAnsTime);

	return avrgAnsTime;
}

int SqliteDatabase::getNumOfCorrectAnswers(std::string username)
{
	std::string query = "SELECT numOfCorrectAnswers FROM Statistics WHERE username LIKE '" + username + "';";
	float num;
	execCommand(query, getFloatCallback, &num);
	return (int)num;
}

int SqliteDatabase::getNumOfTotalAnswers(std::string username)
{
	std::string query = "SELECT TotalAnswers FROM Statistics WHERE username LIKE '" + username + "';";
	float answers;
	execCommand(query, getFloatCallback, &answers);
	return (int)answers;
}

int SqliteDatabase::getPlayerScore(std::string username)
{
	//score calculation is done at insertion
	std::string query = "SELECT Score FROM Statistics WHERE username LIKE '" + username + "';";
	float answers;
	execCommand(query, getFloatCallback, &answers);
	return (int)answers;
}

int SqliteDatabase::getTotalGamesPlayed(std::string username)
{
	std::string query = "SELECT playedGames FROM Statistics WHERE username LIKE '" + username + "';";
	float games;
	execCommand(query, getFloatCallback, &games);
	return (int)games;
}

std::vector<std::string> SqliteDatabase::getHighScores()
{
	std::string query = "SELECT username, Score FROM Statistics ORDER BY Score DESC LIMIT 5;";
	auto vec = new std::vector<std::string>();
	execCommand(query, callback, vec);
	//gets the top 5 names and their scores. so: <name, score, name, score...>
	return *vec;
}


void SqliteDatabase::submitGameStatistics(PlayerResults data, std::string name)
{
	//add the stats to the previus stats
	int score = getPlayerScore(name) + data.score;
	int correct = getNumOfCorrectAnswers(name) + data.correctAnswerCount;

	int total = correct + data.wrongAnswerCount + getNumOfTotalAnswers(name);
	int avrg = (getPLayerAverageAnswerTime(name) + data.averageAnswerTime) / total;

	//insert them
	std::string query = "UPDATE Statistics SET AverageAnswerTime = " + std::to_string(avrg) + ", numOfCorrectAnswers = " + std::to_string(correct) +
		", TotalAnswers = " + std::to_string(total) + ", playedGames = " + std::to_string(getTotalGamesPlayed(name) + 1) + ", Score = " + std::to_string(score) +
		" WHERE Username LIKE '" + name + "';";

	std::cout << query << "\n";

	execCommand(query, nullptr, nullptr);
}


int SqliteDatabase::callback(void* data, int argc, char** argv, char** azColName)
{
	auto vector = (std::vector<std::string>*)data;
	for (int i = 0; argc > i; i++)
	{
		vector->push_back(argv[i]);
	}
	return 0;
}

int SqliteDatabase::getQuestionsCallback(void* questionsRef, int count, char** data, char** azColName)
{
	auto questions = (std::vector<Question>*)questionsRef;

	std::string question = data[0];
	std::string answer = data[1];
	std::string ans1 = data[2];
	std::string ans2 = data[3];
	std::string ans3 = data[4];
		
	auto otherAnswers = std::vector<std::string>();
	otherAnswers = { ans1, ans2, ans3 };
	Question Q = Question(question, answer, otherAnswers);

	questions->push_back(Q);
	return 0;
}

int SqliteDatabase::getFloatCallback(void* floatRef, int count, char** data, char** azColName)
{
	float* num = (float*)floatRef;
	*num = atoi(data[0]);
	return 0;
}
