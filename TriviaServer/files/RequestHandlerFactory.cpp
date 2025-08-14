#include "RequestHandlerFactory.h"
#include "IRequestHandler.h"

RequestHandlerFactory::RequestHandlerFactory(IDatabase* database)
{
	m_database = database;
	m_loginManager = LoginManager(m_database);
	m_statisticsManager = StatisticsManager(m_database);
	m_gameManager = GameManager(m_database);
}

RequestHandlerFactory::~RequestHandlerFactory()
{
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(this, &m_loginManager);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return m_loginManager;
}

MenuRequestHandler* RequestHandlerFactory::CreateMenuRequestHandler(LoggedUser user)
{
	return new MenuRequestHandler(this, user);
}

StatisticsManager* RequestHandlerFactory::getStatisticsManager()
{
	return &m_statisticsManager;
}

RoomManager& RequestHandlerFactory::getRoomManager()
{
	return m_roomManager;
}

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(LoggedUser LU , Room& R)
{
	return new RoomAdminRequestHandler(this ,R ,LU );
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(LoggedUser LU, Room& R)
{
	return new RoomMemberRequestHandler(this, R, LU);
}

GameManager& RequestHandlerFactory::getGameManager()
{
	return m_gameManager;
}

GameRequestHandler* RequestHandlerFactory::createGameRequestHandler(LoggedUser LU , Room& room)
{
	//if the game exsist
	if (getGameManager().doesGameExsist(room.getRoomData().id))
	{
		return new GameRequestHandler(this, getGameManager().getGames()[room.getRoomData().id], LU);
	}
	//if not 
	else
	{
		return new GameRequestHandler(this, getGameManager().createGame(room), LU);
	}

}
