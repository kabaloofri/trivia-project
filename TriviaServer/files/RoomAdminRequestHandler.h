#pragma once
#include "IRequestHandler.h"

class RoomAdminRequestHandler : public IRequestHandler
{
public:
	RoomAdminRequestHandler(RequestHandlerFactory*, Room&, LoggedUser);
	virtual ~RoomAdminRequestHandler() = default;

	bool isRequestRelevant(RequestInfo) const override;
	RequestResult handleRequest(RequestInfo) override;

private:
	RequestResult closeRoom();
	RequestResult startGame();
	RequestResult getRoomState();
	RequestResult changeRoomSettings(RequestInfo req);

	Room* m_room;
	LoggedUser m_user;
};