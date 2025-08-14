#pragma once
#include "GameManager.h"
#include "IRequestHandler.h"

class GameRequestHandler : public IRequestHandler
{
private:
	Game& m_game;
	LoggedUser m_user;

	RequestResult getQuestion(RequestInfo);
	RequestResult submitAnswer(RequestInfo);
	RequestResult getGameResults(RequestInfo);
	RequestResult leaveGame(RequestInfo);

	void waitForPlayersToAnswer(int);

public:
	GameRequestHandler(RequestHandlerFactory* RHF, Game&, LoggedUser); //this create game
	virtual bool isRequestRelevant(RequestInfo) const override;
	virtual RequestResult handleRequest(RequestInfo) override;


};