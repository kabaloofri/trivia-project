#include "Question.h"
#include <algorithm>
#include <random>

Question::Question(std::string question, std::string answer, std::vector<std::string> otherAnswers)
{
    m_question = question;
    otherAnswers.push_back(answer); //set answer to be the last in the list (will randomize later)

    m_correctAnswerIndex = 3;
    m_possibleAnswers = otherAnswers;
}

Question::~Question()
{
}

std::string Question::getQuestion()
{
    return m_question;
}


//returns all questions with a ; seperating them (maybe should return vector)
std::vector<std::string> Question::getPossibleAnswers()
{
    return m_possibleAnswers;
}

int Question::getCorrectAnswerID()
{
    return m_correctAnswerIndex;
}

void Question::mixAnswers()
{
    std::string answer = m_possibleAnswers[getCorrectAnswerID()];
    // mix the answers
    
    std::random_device device;
    std::mt19937 rnd(device());

    std::shuffle(m_possibleAnswers.begin(), m_possibleAnswers.end(), rnd);
    
    for (int i = 0; i < m_possibleAnswers.size(); i++)
    {
        if (m_possibleAnswers[i] == answer)
        {
            m_correctAnswerIndex = i; //set correct answer index again
            break;
        }
    }
    
}
