#pragma once

#include "SqliteDatabase.h"

#include "LoginManager.h"
#include "roomManager.h"
#include "StatisticsManager.h"

#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "GameRequestHandler.h"

class RequestHandlerFactory
{
public:
	RequestHandlerFactory(IDatabase* database);
	RequestHandlerFactory() = default;
	~RequestHandlerFactory();

	LoginRequestHandler* createLoginRequestHandler();

	LoginManager& getLoginManager();

	MenuRequestHandler* CreateMenuRequestHandler(LoggedUser);

	StatisticsManager* getStatisticsManager();

	RoomManager& getRoomManager();

	RoomAdminRequestHandler* createRoomAdminRequestHandler(LoggedUser , Room&);

	RoomMemberRequestHandler* createRoomMemberRequestHandler(LoggedUser , Room&);

	GameManager& getGameManager();

	GameRequestHandler* createGameRequestHandler(LoggedUser, Room&);

private:
	LoginManager m_loginManager;
	IDatabase* m_database;
	RoomManager m_roomManager;
	StatisticsManager m_statisticsManager;
	GameManager m_gameManager;
};