#pragma once
#include "IRequestHandler.h"

class RoomMemberRequestHandler : public IRequestHandler
{
public:
	RoomMemberRequestHandler(RequestHandlerFactory*, Room&, LoggedUser);
	virtual ~RoomMemberRequestHandler() = default;

	bool isRequestRelevant(RequestInfo) const override;
	RequestResult handleRequest(RequestInfo) override;

private:
	RequestResult leaveRoom();
	RequestResult getRoomState();

	Room* m_room;
	LoggedUser m_user;
};

