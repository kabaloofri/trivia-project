#include "RoomMemberRequestHandler.h"

#include "RequestHandlerFactory.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(RequestHandlerFactory* RHF, Room& room, LoggedUser user) 
	: IRequestHandler(RHF), m_room(&room), m_user(user)
{}

bool RoomMemberRequestHandler::isRequestRelevant(RequestInfo request) const
{
	return (request.id == code_leaveRoom || request.id == code_getRoomState);
}

RequestResult RoomMemberRequestHandler::handleRequest(RequestInfo request)
{
	RequestResult res;
	try
	{

		if (isRequestRelevant(request) == false)
		{
			std::string error = "request is not relevent for room member! request code: " + std::to_string(request.id);
			throw std::exception(error.c_str());
		}

		if (request.id == code_leaveRoom)
		{
			res = leaveRoom();
		}
		else
		{
			res = getRoomState();
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "error in handling room member request!- " << e.what();
		ErrorResponse ER;
		ER.message = "error in handling room member request!- ";
		ER.message += e.what();
		res.responce = JsonRequestPacketSerializer::serializeResponse(ER);
		res.newHandler = nullptr;
	}
	return res;
}

RequestResult RoomMemberRequestHandler::leaveRoom()
{
	RequestResult res;
	LeaveRoomResponse LR;

	m_room->removeUser(m_user); //remove the user

	LR.status = status_OK;

	res.responce = JsonRequestPacketSerializer::serializeResponse(LR);//serialzie response
	res.newHandler = m_handlerFactory->CreateMenuRequestHandler(m_user); //go back to menu
	return res;
}

RequestResult RoomMemberRequestHandler::getRoomState()
{
	GetRoomStateResponse stateRes;
	RequestResult res;

	//handle request
	RoomData d = m_room->getRoomData();

	stateRes.status = d.status;
	stateRes.timeUntilGameBegins = d.status == state_gameStarted;
	stateRes.players = std::vector<std::string>(m_room->getAllUsers());
	stateRes.querstionsCount = d.numOfQuestionsInGame;
	stateRes.answerTimeout = d.timePerQuestion;

	//serialize response
	if (stateRes.status == state_begginingGame)
	{
		res.newHandler = m_handlerFactory->createGameRequestHandler(m_user, *m_room); 
	}
	else
	{
		res.newHandler = this;
	}
	res.responce = JsonRequestPacketSerializer::serializeResponse(stateRes);

	return res;
}


