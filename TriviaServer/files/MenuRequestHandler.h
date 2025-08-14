#pragma once

#include "IRequestHandler.h"
#include "LoggedUser.h"
#include "roomManager.h"


class MenuRequestHandler : public IRequestHandler
{
public:
	MenuRequestHandler(RequestHandlerFactory* RHF, LoggedUser user);
	virtual ~MenuRequestHandler();

	bool isRequestRelevant(RequestInfo) const override;
	RequestResult handleRequest(RequestInfo) override;

	std::string getUsername() { return m_user.getUsername(); }

private:
	RequestResult logout(RequestInfo&);
	RequestResult getRooms(RequestInfo&);
	RequestResult getPlayersInRoom(RequestInfo&);
	RequestResult getPersonalStats(RequestInfo&);
	RequestResult getHighScore(RequestInfo&);
	RequestResult joinRoom(RequestInfo&);
	RequestResult createRoom(RequestInfo&);

	LoggedUser m_user;
};

