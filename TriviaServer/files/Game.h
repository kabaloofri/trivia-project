#pragma once
#include "Question.h"
#include <map>


constexpr const float maxPossiblePoints = 1000;

struct GameData
{
	Question currentQuestion;
	unsigned int questionIndex;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	float averageAnswerTime;
	unsigned int currentScore;

	GameData(Question curr)
	{
		currentQuestion = curr;
		questionIndex = 0;
		correctAnswerCount = 0;
		wrongAnswerCount = 0;
		averageAnswerTime = 0;
		currentScore = 0;
	}
	GameData() = default;
};

class Game
{
public:
	Game(std::vector<Question> questions, std::vector<std::string> players, unsigned int ID, float timePerQuestion);
	Game() = default;
	~Game() = default;


	Question getQuestionForUser(std::string);
	void submitAnswer(std::string player, int answerID, float answerTime);
	void removePlayer(std::string);
	int getAnswer(std::string user);

	unsigned int getID();
	std::map<std::string, GameData>& getPlayers() { return m_players; };
	void increaseQuestionIndex(std::string);
private:


	std::vector<Question> m_questions;
	std::map<std::string, GameData> m_players;

	unsigned int m_gameID;
	float _timePerQuestion;
};

