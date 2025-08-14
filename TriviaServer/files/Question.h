#pragma once
#include <string>
#include <vector>

class Question
{
public:
	Question(std::string question, std::string answer, std::vector<std::string> otherAnswers);
	Question() = default;
	~Question();

	std::string getQuestion();
	std::vector<std::string> getPossibleAnswers();
	int getCorrectAnswerID();

	void mixAnswers();

private:
	std::string m_question;
	std::vector<std::string> m_possibleAnswers;
	int m_correctAnswerIndex;
};

