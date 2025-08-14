#include "RoomAdminRequestHandler.h"

#include "RequestHandlerFactory.h"
#include "JsonRequestPacketDeserializer.h"
#include <string>
#include "Room.h"

#include <thread>
#include <chrono>

RoomAdminRequestHandler::RoomAdminRequestHandler(RequestHandlerFactory* RHF, Room& room, LoggedUser user) 
	: IRequestHandler(RHF), m_room(&room), m_user(user)
{}

bool RoomAdminRequestHandler::isRequestRelevant(RequestInfo request) const
{
	return (request.id == code_closeRoom || request.id == code_startGame 
		|| request.id == code_getRoomState || request.id == code_changeSettings);
}

RequestResult RoomAdminRequestHandler::handleRequest(RequestInfo request)
{
	RequestResult res;
	try
	{
		if (isRequestRelevant(request) == false)
		{
			throw std::exception("request is not relevent for admin! request code: " + request.id);
		}

		switch (request.id)
		{
		case code_closeRoom:
			res = closeRoom();
			break;
		case code_startGame:
			res = startGame();
			break;
		case code_getRoomState:
			res = getRoomState();
			break;
		case code_changeSettings:
			res = changeRoomSettings(request);
			break;
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "error in handling admin request!- " << e.what();
		ErrorResponse ER;
		ER.message = "error in handling admin request!- ";
		ER.message += e.what();
		res.responce = JsonRequestPacketSerializer::serializeResponse(ER);
		res.newHandler = nullptr;
	}
	return res;
}

RequestResult RoomAdminRequestHandler::closeRoom()
{
	CloseRoomResponse CR;
	RequestResult res;

	m_room->setStatus(state_inactive);


	while (true) //wait until all players go
	{
		if (m_room->getRoomData().currentAmountOfPlayers ==1)
		{
			m_room->removeUser(m_user); //remove the admin from the room
			m_handlerFactory->getRoomManager().deleteRoom(m_room->getRoomData().id); //delete the room
			break;
		}
	}

	CR.status = status_OK;


	res.responce = JsonRequestPacketSerializer::serializeResponse(CR);
	res.newHandler = m_handlerFactory->CreateMenuRequestHandler(m_user);

	return res;
}

RequestResult RoomAdminRequestHandler::startGame()
{
	StartGameResponse SGR;
	RequestResult res;
	
	m_room->startGame();


	//timer would start now. every time in the game state would now come a time until the game starts.
	//every client get the time at different times, but will start in the same time.
	//once a client recives the status a game is starting, he would get the time and count until the game starts, and move to the game screan

	SGR.status = status_OK;
	res.responce = JsonRequestPacketSerializer::serializeResponse(SGR);
	res.newHandler = m_handlerFactory->createGameRequestHandler(m_user , *m_room); //room is in countdown so stay in here
	return res;
}

RequestResult RoomAdminRequestHandler::getRoomState()
{
	GetRoomStateResponse stateRes;
	RequestResult res;

	//handle request
	RoomData d = m_room->getRoomData();

	stateRes.status = d.status;
	stateRes.timeUntilGameBegins = m_room->getGameStartTime();
	stateRes.players = std::vector<std::string>(m_room->getAllUsers());
	stateRes.querstionsCount = d.numOfQuestionsInGame;
	stateRes.answerTimeout = d.timePerQuestion;

	//serialize response
	res.responce = JsonRequestPacketSerializer::serializeResponse(stateRes);
	res.newHandler = this;

	return res;
}

RequestResult RoomAdminRequestHandler::changeRoomSettings(RequestInfo req)
{
	RequestResult reqRes;
	ChangeRoomSettings settings;
	
	//deserialize request
	settings = JsonRequestPacketDeserializer::deserializeChangeRoomRequest(req.buffer.getStrBuffer());

	//handle request
	m_room->changeSettings(settings.questionCount, settings.answerTimeout);

	//serialize response
	ChangeRoomSettingsResponse res;
	res.status = status_OK;
	
	reqRes.responce = JsonRequestPacketSerializer::serializeResponse(res);
	reqRes.newHandler = this;
	return reqRes;
}
