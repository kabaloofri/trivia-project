#include "Game.h"
#include <algorithm>

#include <iostream>

Game::Game(std::vector<Question> questions, std::vector<std::string> players, unsigned int ID, float timePerQuestion)
{
	m_gameID = ID;
	this->_timePerQuestion = timePerQuestion;

	m_questions = std::vector<Question>(questions);

	GameData data = GameData(m_questions[0]); //start at first question

	for (int i = 0; i < players.size(); i++)
	{
		m_players.emplace(players[i], data);
	}
}

Question Game::getQuestionForUser(std::string player)
{
	// get current room data
	auto data = &m_players[player];
	
	if (data->questionIndex > m_questions.size())
	{
		//player has reached the end of the questions
		std::cout << "player " << player << " has finished all questions!\n";

		removePlayer(player); //remove the player?
	}	
	else
	{
		data->currentQuestion = m_questions[data->questionIndex];
		data->currentQuestion.mixAnswers(); //mix the possible answers of the question

		data->questionIndex++;
	}

	return data->currentQuestion;
}

void Game::submitAnswer(std::string player, int answerID, float answerTime)
{
	auto data = &m_players[player];
	
	bool isAnswerCorrect;
	
	if (data->currentQuestion.getCorrectAnswerID() == answerID)
	{
		data->correctAnswerCount++;
		isAnswerCorrect = true;
	}
	else
	{
		data->wrongAnswerCount++;
		isAnswerCorrect = false;
	}
	//add the time to the average - (average + new)  / (total answers)
	data->averageAnswerTime = (data->averageAnswerTime + answerTime) / (data->correctAnswerCount + data->wrongAnswerCount);
	
	//calculate score
	if (isAnswerCorrect)
	{
		//only update score if the answer was correct- wrong answers give 0 points

		//score calculation:
		// | ((answerTime / timePerQuestion) / 2) - 1 | * 1000

		if (answerTime == 0)
		{
			answerTime = 0.1;
		}
		double temp = answerTime / _timePerQuestion;
		temp /= 2;
		temp = abs(temp - 1);

		temp *= maxPossiblePoints;

		int score = (int)temp;

		data->currentScore += score; //add score
	}
}

void Game::removePlayer(std::string player)
{
	m_players.erase(player);
}

int Game::getAnswer(std::string user)
{
	return m_players[user].currentQuestion.getCorrectAnswerID();

}

unsigned int Game::getID()
{
	return m_gameID;
}


void Game::increaseQuestionIndex(std::string player)
{
	auto data = &m_players[player];
	data->questionIndex++;
}

